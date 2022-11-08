/**
 * @file Iaxis_motion.h
 * @author sami dhiab
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "linear_motion.h"

linear_motion::linear_motion(/* args */)
{
    std::cout << "creating linear axis  client" <<std::endl;
}

linear_motion::~linear_motion()
{
    if(axis_client_sock!=nullptr) delete axis_client_sock;

}
 void linear_motion::sendDirectCmd(std::string& cmd) 
 {
        if (axis_client_sock->write(cmd ) != ssize_t(std::string(cmd ).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << axis_client_sock->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd  << " sent" << std::endl;
waitForResponse();
 }
 
bool linear_motion::getStatus()
{
    return axisReady;
}


/**
 * @brief TODO: change the function call
 * 
 * @param new_position 
 */
void linear_motion::move_to(int new_position) 
{

    move_up_by(new_position);
    
}

wgm_feedbacks::enum_sub_sys_feedback linear_motion::connect() 
{
     std::cout << "connecting controller to axis server" << std::endl;
    axis_client_sock = new sockpp::tcp_connector({ _motion_axis_struct.ip, _motion_axis_struct.port });

    // Implicitly creates an inet_address from {host,port}
    // and then tries the connection.
    if (!axis_client_sock->is_connected()) {
        std::cerr << "Error connecting to axis server at "
            << sockpp::inet_address(_motion_axis_struct.ip, _motion_axis_struct.port)
            << " -> " << axis_client_sock->last_error_str();
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << axis_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << axis_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!axis_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << axis_client_sock->last_error_str() << std::endl;
        axisReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    axisReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

void linear_motion::disconnect() 
{
    axis_client_sock->close();
}


/**
 * @brief
 *
 * @return double
 */
double linear_motion::get_position()
{
    double axis_pos = 0;
    std::cout << "get axis curent position" << std::endl;
    auto command = axis_cmds.find(1);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendCmd(command->second, axis_client_sock);
    }
    std::cout << "awaiting server response" << std::endl;

    while (axis_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = axis_client_sock->read(&axis_incoming_data[0], 1024);
        //std::cout << "n bytes: " << n << std::endl;
        //std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << axis_incoming_data.c_str() << std::endl;
            axis_pos = atof(axis_incoming_data.c_str()); // to double
            std::cout << "filter val : " << axis_pos << std::endl;
            axis_last_position.push_front(axis_pos); // add to table

            std::cout << "value added to table " << axis_last_position.front() << std::endl;
            break;
        }
    }
    return axis_pos;
}
/**
 * @brief
 *
 */
void linear_motion::move_home()
{
      auto command = axis_cmds.find(7);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second <<  '\n';
        sendCmd(command->second, axis_client_sock);
    }

}
/**
 * @brief
 *
 */
void linear_motion::get_speed()
{

}
void linear_motion::set_speed(double_t new_val)
{

}
void linear_motion::move_up_to(double_t new_pos)
{

}
void linear_motion::move_down_to(double_t new_pos)
{

}
/**
 * @brief
 *
 * @param steps
 */
void linear_motion::move_up_by(double_t steps)
{
    std::cout << "moving up by " << steps << std::endl;
    auto command = axis_cmds.find(5);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = "-" + std::to_string(steps);
        sendCmd(command->second, axis_client_sock, args);
    }
    std::cout << "awaiting server response" << std::endl;

    while (axis_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = axis_client_sock->read(&axis_incoming_data[0], 1024);
        // std::cout << "n bytes: " << n << std::endl;
        // std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << axis_incoming_data.c_str() << std::endl;
            std::cout << "axis is moving up " << std::endl;
            break;
        }
    }
}

/**
 * @brief
 *
 * @param steps
 */
void linear_motion::move_down_by(double_t steps)
{

    std::cout << "moving down by " << steps << std::endl;
    auto command = axis_cmds.find(6);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << steps << '\n';
        std::string args = std::to_string(steps);
        sendCmd(command->second, axis_client_sock, args);
    }
    std::cout << "awaiting server response" << std::endl;

    while (axis_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = axis_client_sock->read(&axis_incoming_data[0], 1024);
        // std::cout << "n bytes: " << n << std::endl;
        // std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
        if (n > 0)
        {
            std::cout << "server replied : " << axis_incoming_data.c_str() << std::endl;
            std::cout << "axis is moving down " << std::endl;
            break;
        }
    }

}

void linear_motion::waitForResponse()
{
    std::cout << "awaiting server response" << std::endl;

    while (axis_client_sock->is_connected())
    {
        // Read_n data from keyence
        ssize_t n = axis_client_sock->read(&axis_incoming_data[0], axis_data_length);
        if (n > 0)
        {
            std::cout << "server replied : " << axis_incoming_data.c_str() << std::endl;
            break;
        }
    }
}


void linear_motion::move_center()
{
    auto command = axis_cmds.find(2);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second <<  '\n';
        sendCmd(command->second, axis_client_sock);
    }
}

void linear_motion::unlock()
{
    auto command = axis_cmds.find(0);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second <<  '\n';
        sendCmd(command->second, axis_client_sock);
    }

}