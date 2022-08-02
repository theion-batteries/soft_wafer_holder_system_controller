#include "whs_controller.h"
/*********** constructors and desctructions **********/

whs_controller::whs_controller(/* args */)
{
    std::cout << "creating wafer holder system controller " << std::endl;
    std::cout << "connect from ip: " <<_keyence_struct.ip<< "port "<< _keyence_struct.port<< std::endl;
    keyence_client_sock = new sockpp::tcp_connector({ "127.0.0.1", 6555 });

}

whs_controller::~whs_controller()
{
    delete delta_client_sock;
    delete keyence_client_sock;
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
void whs_controller::get_keyence_sensor_mesured_Values()
{
    if (keyence_last_mesured.size()>10) keyence_last_mesured.pop_back(); // remove last if 10
    double current_value = 0;
    auto command = keyence_cmds.find(1);
    if (command != keyence_cmds.end()) {
            std::cout << "Found " << command->first << " " << command->second << '\n';
            sendCmd(command->second, keyence_client_sock);
    }
    while (keyence_client_sock->is_connected())
        {
            // Read data from keyence
            ssize_t hasData = keyence_client_sock->read(keyence_incoming_data, keyence_data_length);
            if (hasData)
            {
                current_value = std::stod(std::string(keyence_incoming_data)); // convert the data to double
                keyence_last_mesured.push_back(current_value); // add to table
                break;
            }
            Sleep(200); // do this every 200 ms
        }
}

