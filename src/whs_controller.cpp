#include "whs_controller.h"
/*********** constructors and desctructions **********/

whs_controller::whs_controller(/* args */)
{
    std::cout << "creating wafer holder system controller " << std::endl;
}

whs_controller::~whs_controller()
{
    delete delta_client_sock;
    delete keyence_client_sock;
}
void whs_controller::close_all_sockets()
{
    if (!keyence_client_sock) keyence_client_sock->close();
    if (!delta_client_sock) delta_client_sock->close();

}

/********* run subprocesses *******/
void whs_controller::run_delta_subprocess() {
    std::cout << "Running delta program " << std::endl;
    ShellExecuteW(NULL, L"open", pyCmd, pyFilePath, NULL, SW_SHOWDEFAULT);
}
void whs_controller::run_keyence_subprocess() {
    std::cout << "Running keyence program " << std::endl;
    ShellExecuteW(NULL, L"open", cppFile, NULL, NULL, SW_SHOWDEFAULT);
}
void whs_controller::run_all_subprocesses()
{
    run_delta_subprocess();
    run_keyence_subprocess();
}

void whs_controller::sendCmd(std::string cmd, sockpp::tcp_connector* client)
{
    if (client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd << " sent" << std::endl;
}
/****** connect to kyence bridge server *******/

void whs_controller::connect_to_keyence_server()
{
    std::cout << "connecting controller to keyence server" << std::endl;
    keyence_client_sock = new sockpp::tcp_connector({ _keyence_struct.ip, _keyence_struct.port });
    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!keyence_client_sock) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_keyence_struct.ip, _keyence_struct.port)
            << "\n\t" << keyence_client_sock->last_error_str() << std::endl;
        return;
    }
    std::cout << "Created a connection from " << keyence_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << keyence_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!keyence_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << keyence_client_sock->last_error_str() << std::endl;
    }
}

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
void whs_controller::keyence_client_connect()
{
    std::cout << "Running keyence client " << std::endl;
    Kclient = new keyence_client(keyence_ip);
    Kclient->connect();
}

double  whs_controller::keyence_client_get_value_output0()
{
    std::cout << "get reading keyence sensor output0" << std::endl;
    if (keyence_last_mesured_output0.size() > 10) keyence_last_mesured_output0.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(0);
    if (current_value == 0) return 0;
    keyence_last_mesured_output0.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output0.front() << std::endl;
}
double  whs_controller::keyence_client_get_value_output1()
{

    std::cout << "get reading keyence sensor output1" << std::endl;
    if (keyence_last_mesured_output1.size() > 10) keyence_last_mesured_output1.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(1);
    if (current_value == 0) return 0;
    keyence_last_mesured_output1.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output1.front() << std::endl;
}
double  whs_controller::keyence_client_get_value_output2()
{

    std::cout << "get reading keyence sensor output2" << std::endl;
    if (keyence_last_mesured_output2.size() > 10) keyence_last_mesured_output2.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(2);
    if (current_value == 0) return 0;
    keyence_last_mesured_output2.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output2.front() << std::endl;
}
void whs_controller::keyence_client_get_value_all()
{
    keyence_client_get_value_output0();
    keyence_client_get_value_output1();
    keyence_client_get_value_output2();
    keyenceReady = true;
}


/**************** Algorithms conntroller ***************/


void whs_controller::move_down_until_data_availble()
{
    while (keyence_client_get_value_output0()==0) // while data invalid, we go down further
    {
    std::cout << "moving down until reading values " << std::endl;
    
    }
    


}

























/****************** Delta repetier server methods **********/

void whs_controller::connect_to_delta_server()
{

    std::cout << "connecting controller to delta server" << std::endl;
    delta_client_sock = new sockpp::tcp_connector({ _delta_struct.ip, _delta_struct.port });

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!delta_client_sock) {
        std::cerr << "Error connecting to server at "
            << sockpp::inet_address(_delta_struct.ip, _delta_struct.port)
            << "\n\t" << delta_client_sock->last_error_str() << std::endl;
        return;
    }
    std::cout << "Created a connection from " << delta_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << delta_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!delta_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << delta_client_sock->last_error_str() << std::endl;
    }
}
void whs_controller::get_delta_position()
{
    std::cout << "get delta curent position" << std::endl;
    if (delta_last_position.size() > 10) delta_last_position.pop_back(); // remove last if 10

    auto command = delta_cmds.find(1);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, delta_client_sock);
    }
    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read_n(&delta_incoming_data[0], 1024);
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied succeffully: " << delta_incoming_data.c_str() << std::endl;
            double delta_pos = atof(delta_incoming_data.c_str());
            delta_last_position.push_back(delta_pos); // add to table
            std::cout << "value added to table " << delta_last_position.front() << std::endl;
            break;
        }
    }
}

void whs_controller::move_delta_home()
{
    //if (delta_last_position.front()==300) return ; // already homed
    auto command = delta_cmds.find(7);
    if (command != delta_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, delta_client_sock);
    }
    while (delta_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = delta_client_sock->read_n(&delta_incoming_data[0], 1024);
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied succeffully: " << delta_incoming_data.c_str() << std::endl;
            std::cout << "delta is ready " << std::endl;
            deltaReady = true;
            break;
        }
    }

}

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
void whs_controller::move_delta_up_by(double_t steps)
{

}
void whs_controller::move_delta_down_by(double_t steps)
{

}