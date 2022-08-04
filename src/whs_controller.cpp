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
    if(!keyence_client_sock) keyence_client_sock->close();
    if(!delta_client_sock) delta_client_sock->close();

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

/****** connect client to server *******/
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
void whs_controller::sendCmd(std::string cmd, sockpp::tcp_connector* client)
{
    if (client->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd << " sent" << std::endl;
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
        ssize_t n = keyence_client_sock->read_n(&keyence_incoming_data[0], 1024);
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        //if (n != ssize_t(command->second.length())) {
        //    std::cerr << "Error reading from TCP stream: "
        //        << keyence_client_sock->last_error_str() << std::endl;
        //    break;
        //}
        std::cout << "server replied succeffully: " << keyence_incoming_data.c_str() << std::endl;
        current_value = std::stod(keyence_incoming_data); // convert the data to double
        keyence_last_mesured.push_back(current_value); // add to table
        std::cout << "value added to table " <<  keyence_last_mesured.front() << std::endl;
        break;
    }
}

void whs_controller::get_delta_position()
{
    std::cout << "get delta curent position" << std::endl;

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
        if (n>0)
        {
        std::cout << "server replied succeffully: " << delta_incoming_data.c_str() << std::endl;
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