/**
 * @file whs_controller.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-08-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "whs_controller.h"
#include <fstream>


whs_controller::whs_controller()
{
    std::cout << "creating wafer holder motion controller " << std::endl;
#ifdef WHS_CONFIG
    std::cout << WHS_CONFIG << std::endl;
    std::cout << "loading config file" << std::endl;
    std::ifstream filein(WHS_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(WHS_CONFIG);
    _whs_params.fi = (config["script"].as<std::string>());
    _whs_params.py = (config["interpreter"].as<std::string>());
    _whs_params.cmd = _whs_params.py + " " + _whs_params.fi;
    _whs_params.pyCmd = &_whs_params.cmd[0];
    _whs_params.pyFile = &_whs_params.fi[0];
    _whs_params.pyInterpreter = _whs_params.py.c_str();
    _whs_params.mm_steps = config["mm_steps"].as<double>();
    _whs_params.mm_step_res = config["mm_step_res"].as<double>();
    _whs_params.ref_dis = config["ref_dis"].as<double>();
    _whs_params.delay_to_move_request = config["delay_to_move_request"].as<DWORD>();
    _whs_params.thickness = config["thickness"].as<double>();

#endif 
}
/**
 * @brief Destroy the whs controller::whs controller object
 *
 */
whs_controller::~whs_controller()
{
    if (delta_client_sock != nullptr) delete delta_client_sock;
    if (Kclient != nullptr) delete Kclient;
}
/**
 * @brief
 *
 */
void whs_controller::close_all_sockets()
{
    if (!keyence_client_sock) keyence_client_sock->close();
    if (!delta_client_sock) delta_client_sock->close();

}

/********* run subprocesses *******/

/**
 * @brief
 *
 */
void whs_controller::run_delta_subprocess() {
    std::cout << "Running delta program in new console" << std::endl;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process. 
    if( !CreateProcess(_whs_params.pyInterpreter,   // No module name (use command line)
        _whs_params.pyCmd,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, NULL );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

}
/**
 * @brief
 *
 */
void whs_controller::run_keyence_subprocess() {
    std::cout << "Running keyence program " << std::endl;
}
/**
 * @brief
 *
 */
void whs_controller::run_all_subprocesses()
{
    run_delta_subprocess();
    run_keyence_subprocess();
}
/**
 * @brief
 *
 * @param cmd
 * @param client
 * @param args
 */
void whs_controller::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}
/****** connect to kyence bridge server *******/

/**
 * @brief
 *
 */
void whs_controller::connect_to_keyence_server()
{
    std::cout << "connecting controller to keyence server" << std::endl;
    keyence_client_sock = new sockpp::tcp_connector({ _keyence_struct.ip, _keyence_struct.port });
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!keyence_client_sock->is_connected()) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_keyence_struct.ip, _keyence_struct.port)
            << "\n\t" << keyence_client_sock->last_error_str() << std::endl;
        keyenceReady = false;
        return;
    }
    std::cout << "Created a connection from " << keyence_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << keyence_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!keyence_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << keyence_client_sock->last_error_str() << std::endl;
        keyenceReady = false;
        return;
    }
    keyenceReady = true;
}
/**
 * @brief
 *
 */
void  whs_controller::set_keyence_mesurement_mode()
{
    std::cout << "set keyence mesuremnent mode" << std::endl;
    auto command = keyence_cmds.find(5);
    if (command != keyence_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, keyence_client_sock);
    }
    while (keyence_client_sock->is_connected())
    {
        // Read data from keyence
        ssize_t n = keyence_client_sock->read_n(&keyence_incoming_data[0], command->second.length());
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n != ssize_t(command->second.length())) {
            std::cerr << "Error reading from TCP stream: "
                << keyence_client_sock->last_error_str() << std::endl;
            break;
        }
        if (keyence_incoming_data.c_str() == command->second) //reply with same msg is success
        {
            std::cout << "server replied succeffully: " << keyence_incoming_data.c_str() << std::endl;
            keyence_incoming_data.clear();

            break;
        }
    }
}
/**
 * @brief
 *
 */
void whs_controller::get_keyence_sensor_mesured_Values()
{
    std::cout << "get reading keyence sensor values" << std::endl;
    if (keyence_last_mesured.size() > 10) keyence_last_mesured.pop_back(); // remove last if 10
    double current_value = 0;
    auto command = keyence_cmds.find(1);
    if (command != keyence_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, keyence_client_sock);
    }
    while (keyence_client_sock->is_connected())
    {
        // Read data from keyence
        ssize_t n = keyence_client_sock->read_n(&keyence_incoming_data[0], command->second.length());
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n != ssize_t(command->second.length())) {
            std::cerr << "Error reading from TCP stream: "
                << keyence_client_sock->last_error_str() << std::endl;
            break;
        }
        std::cout << "server replied succeffully: " << keyence_incoming_data.c_str() << std::endl;
        current_value = std::stod(keyence_incoming_data); // convert the data to double
        keyence_last_mesured.push_back(current_value); // add to table
        std::cout << "value added to table " << keyence_last_mesured.front() << std::endl;
        break;
    }
}

/************************* keyence client methods*****************/

/**
 * @brief
 *
 */
enum_sub_sys_feedback whs_controller::keyence_client_connect()
{
    std::cout << "Running keyence client " << std::endl;
    Kclient = new keyence_client(keyence_ip);
    enum_hw_feedback Keyence_feedback = Kclient->connect();
    if (Keyence_feedback == enum_hw_feedback::hw_success)
    {
        keyenceReady = true;
        return enum_sub_sys_feedback::sub_success;
    }
    else
    {
        keyenceReady = false;
        return enum_sub_sys_feedback::sub_error;
    }



}
/**
 * @brief
 *
 * @return double
 */
double  whs_controller::keyence_client_get_value_output0()
{
    std::cout << "get reading keyence sensor output0" << std::endl;
    if (keyence_last_mesured_output0.size() > 10) keyence_last_mesured_output0.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(0);
    if (current_value == 0) return 0;
    keyence_last_mesured_output0.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output0.front() << std::endl;
    return current_value;

}

/**
 * @brief
 *
 * @return double
 */
double  whs_controller::keyence_client_get_value_output1()
{

    std::cout << "get reading keyence sensor output1" << std::endl;
    if (keyence_last_mesured_output1.size() > 10) keyence_last_mesured_output1.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(1);
    if (current_value == 0) return 0;
    keyence_last_mesured_output1.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output1.front() << std::endl;
    return current_value;

}
/**
 * @brief
 *
 * @return double
 */
double  whs_controller::keyence_client_get_value_output2()
{

    std::cout << "get reading keyence sensor output2" << std::endl;
    if (keyence_last_mesured_output2.size() > 10) keyence_last_mesured_output2.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(2);
    if (current_value == 0) return 0;
    keyence_last_mesured_output2.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output2.front() << std::endl;
    return current_value;

}
/**
 * @brief
 *
 */
void whs_controller::keyence_client_get_value_all()
{
    keyence_client_get_value_output0();
    keyence_client_get_value_output1();
    keyence_client_get_value_output2();
}

/****************** Delta repetier server methods **********/


/**
 * @brief
 *
 */
enum_sub_sys_feedback whs_controller::connect_to_delta_server()
{

    std::cout << "connecting controller to delta server" << std::endl;
    delta_client_sock = new sockpp::tcp_connector({ _delta_struct.ip, _delta_struct.port });

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!delta_client_sock->is_connected()) {
        std::cerr << "Error connecting to delta server at "
            << sockpp::inet_address(_delta_struct.ip, _delta_struct.port)
            << " -> " << delta_client_sock->last_error_str();
        deltaReady = false;
        return enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << delta_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << delta_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!delta_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << delta_client_sock->last_error_str() << std::endl;
        deltaReady = false;
        return enum_sub_sys_feedback::sub_error;
    }
    deltaReady = true;
    return enum_sub_sys_feedback::sub_success;


}
/**
 * @brief
 *
 * @return double
 */
double whs_controller::get_delta_position()
{
    double delta_pos = 0;
    std::cout << "get delta curent position" << std::endl;
    if (!delta_last_position.empty()) {
        if (delta_last_position.size() > 10) delta_last_position.pop_back(); // remove last if 10
    }
    auto command = delta_cmds.find(1);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, delta_client_sock);
    }
    std::cout << "awaiting server response" << std::endl;

    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read(&delta_incoming_data[0], 1024);
        //std::cout << "n bytes: " << n << std::endl;
        //std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << delta_incoming_data.c_str() << std::endl;
            delta_pos = atof(delta_incoming_data.c_str()); // to double
            std::cout << "filter val : " << delta_pos << std::endl;
            delta_last_position.push_front(delta_pos); // add to table

            std::cout << "value added to table " << delta_last_position.front() << std::endl;
            break;
        }
    }
    return delta_pos;
}
/**
 * @brief
 *
 */
void whs_controller::move_delta_home()
{
    double last_pos = 0;
    if (!delta_last_position.empty())
    {
        last_pos = delta_last_position.front(); // this shall start at 300
        if (last_pos == 300) return; //already
    }
    else {
        last_pos = get_delta_position();
        if (last_pos == 300) return; //already
    }
    //if (delta_last_position.front()==300) return ; // already homed
    auto command = delta_cmds.find(7);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, delta_client_sock);
    }
    std::cout << "awaiting server response" << std::endl;
    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read(&delta_incoming_data[0], 1024);
        // std::cout << "n bytes: " << n << std::endl;
        // std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << delta_incoming_data.c_str() << std::endl;
            std::cout << "delta is ready " << std::endl;
            break;
        }
        if (delta_client_sock->read_timeout(std::chrono::seconds(5))) {
            std::cerr << "Error setting timeout on TCP stream: "
                << delta_client_sock->last_error_str() << std::endl;
            break;
        }
    }
    // mandatory wait for mechanical movement: 1000-2000 ms
    Sleep(2000);

}
/**
 * @brief
 *
 */
void whs_controller::get_delta_speed()
{

}
void whs_controller::set_delta_speed(double_t new_val)
{

}
void whs_controller::move_delta_up_to(double_t new_pos)
{

}
void whs_controller::move_delta_down_to(double_t new_pos)
{

}
/**
 * @brief
 *
 * @param steps
 */
void whs_controller::move_delta_up_by(double_t steps)
{
    std::cout << "moving up by " << steps << std::endl;
    auto command = delta_cmds.find(5);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = " " + std::to_string(steps);
        sendCmd(command->second, delta_client_sock, args);
    }
    std::cout << "awaiting server response" << std::endl;

    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read(&delta_incoming_data[0], 1024);
        // std::cout << "n bytes: " << n << std::endl;
        // std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << delta_incoming_data.c_str() << std::endl;
            std::cout << "delta is moving up " << std::endl;
            break;
        }
    }
}

/**
 * @brief
 *
 * @param steps
 */
void whs_controller::move_delta_down_by(double_t steps)
{

    std::cout << "moving down by " << steps << std::endl;
    auto command = delta_cmds.find(6);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = " " + std::to_string(steps);
        sendCmd(command->second, delta_client_sock, args);
    }
    std::cout << "awaiting server response" << std::endl;

    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read(&delta_incoming_data[0], 1024);
        // std::cout << "n bytes: " << n << std::endl;
        // std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << delta_incoming_data.c_str() << std::endl;
            std::cout << "delta is moving down " << std::endl;
            break;
        }
    }

}


/**************** Algorithms conntroller ***************/

/**
 * @brief
 *
 * @param mm
 * @return double
 */
double whs_controller::calculate_time_to_move_steps(float mm)
{
    return mm * 100; // assume time step
}

void whs_controller::move_down_until_data_availble()
{
    // test version: delta z pos: 125, keyence get data
    double last_pos = 0;
    if (!delta_last_position.empty())
    {
        last_pos = delta_last_position.front(); // this shall start at 300
    }
    else {
        last_pos = get_delta_position();
    }
    std::cout << "start pos:  " << last_pos << std::endl;
    while (keyence_client_get_value_output1() == 0) // while data invalid, we go down further
    {
        std::cout << "moving down by " << _whs_params.mm_steps << "mm_steps until reading values " << std::endl;
        move_delta_down_by(_whs_params.mm_steps); // move down by mm steps
        std::cout << "wait for cmd moving down to finish, asking for new position  " << std::endl;
        Sleep(_whs_params.delay_to_move_request);
    }
}
/**
 * @brief
 *
 * @param ref_dis
 */
void whs_controller::move_down_to_surface()
{
    if (_whs_params.ref_dis == 0) _whs_params.ref_dis = reference_distance;
    std::cout << "moving down to surface" << std::endl;
    std::cout << "moving down until sensor reading is equal the refernce distance: " << _whs_params.ref_dis << std::endl;
    // test version: delta z pos: 125, keyence get data
    double last_pos = 0;
    float mm_steps = 1; // 
    if (!delta_last_position.empty())
    {
        last_pos = delta_last_position.front();
    }
    else {
        last_pos = get_delta_position();
    }
    while (keyence_client_get_value_output1() >= _whs_params.ref_dis) // while keyence reading is <= to reference distance
    {
        std::cout << "moving down by " << mm_steps << "mm_steps until reading values " << std::endl;
        move_delta_down_by(mm_steps); // move down by mm steps
        std::cout << "wait for cmd moving down to finish, asking for new position  " << std::endl;
        Sleep(1000);
        get_delta_position();
        std::cout << "current pos:  " << delta_last_position.front() << std::endl;
        Sleep(500);
    }
}
/**
 * @brief
 *
 * @param thickness
 * @param mm_step_res
 */
void whs_controller::deep_wafer_holder_desired_thickness() //default to 0.01 mm_step x 10 steps= 0.1mm or 100µm
{
    this->thickness = _whs_params.thickness;
    lowest_step_res = _whs_params.mm_step_res;
    unsigned int steps = thickness / _whs_params.mm_step_res;
    for (unsigned int step_counter = 0; step_counter < steps; step_counter++)
    {
        std::cout << "iteration number " << step_counter << std::endl;
        move_delta_down_by(_whs_params.mm_step_res); // move mm step default 0.01 mm
        Sleep(200); //wait for movement to finish
        // to check
        keyence_client_get_value_output1();
        // to verify step res
        get_delta_position();
    }
    waferHolderReady = true;
}
/**
 * @brief function to monitor the distance and calibrate depth
 */
void whs_controller::monitor_and_calibrate()
{
    std::cout << "monitor calibration started" << std::endl;
    // algorithm: if current distance is not equal desired thickness, calibrate
    // 1. check if diff is a value that is below the step resolution, continue or
    // 2. move up if diff negativ
    // 3. move down if diff positiv
    while (true)
    {
        double diff = keyence_client_get_value_output1() - thickness;
        if (abs(diff) <= lowest_step_res) // if the difference mesured lower than min step res, we skip calibration
        {
            continue;
        }
        else
        {
            if (diff < 0) // if diff negativ, we move up
            {
                move_delta_up_by(diff);
            }
            else
            {
                move_delta_down_by(diff);

            }

        }

    }


}


