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
#include <thread>
#include <mutex>
#include <string.h>
linear_motion::linear_motion(/* args */)
{
    std::cout << "creating linear axis  client" << std::endl;
}

linear_motion::~linear_motion()
{
    if (axis_client_sock != nullptr) delete axis_client_sock;

}
std::string linear_motion::sendDirectCmd(std::string cmd)
{
    if (axis_client_sock == nullptr) return "not connected";
    std::cout<< "sending linear axis command "<< cmd <<std::endl;
    cmd = cmd+"\r\n";
    if (axis_client_sock->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cout << "Error writing to the TCP stream: "
            << axis_client_sock->last_error_str() << std::endl;
    }
    return waitForResponse();
}

std::string linear_motion::waitForResponse()
{
    static int attempts = 0;
    if (attempts == 10)
    {
        std::cout << "attempts: " << attempts << std::endl;
        attempts = 0;
        return "NA";
    }
    std::cout << "awaiting server response" << std::endl;
    if (axis_client_sock->is_connected())
    {
        char Strholder[1024];
        ssize_t n = axis_client_sock->read_n(&Strholder, 1024);
        if (n > 0)
        {
            std::cout << "n bytes received: " << n << std::endl;
            axis_incoming_data = Strholder;
            axis_incoming_data.resize(n);
            std::cout << "server replied : " << axis_incoming_data << std::endl;
            return axis_incoming_data;
        }
        else
        {
            std::cout << "no server response " << n << std::endl;
            attempts++;
            return waitForResponse();
        }

    }
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
    std::cout << "sending command: " << command->second << '\n';
    auto resp =sendDirectCmd(command->second);
    std::string extracted = resp.substr(resp.find_first_of(":") + 1, resp.find_first_of(",") - 1 - resp.find_first_of(":"));
    axis_pos = std::stod(extracted); // to double
    std::cout << "filter val : " << axis_pos << std::endl;
    axis_last_position.push_front(axis_pos); // add to table
    std::cout << "value added to table " << axis_last_position.front() << std::endl;
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
        std::cout << "sending command: " << command->second << '\n';
        auto reply = sendDirectCmd(command->second);
        
       // if (!reply.find("ok")) return move_home();
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
    std::cout << "moving down to " << new_pos << std::endl;
    auto command = axis_cmds.find(6);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << " args: " << new_pos << '\n';
        std::string args = std::to_string(new_pos);
        auto cmd = (command->second)+args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
    }
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
        sendDirectCmd(command->second);
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
        auto cmd = (command->second)+args;
        // X-Steps
        auto reply = sendDirectCmd(cmd);
    }
}




void linear_motion::move_center()
{
    auto command = axis_cmds.find(2);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendDirectCmd(command->second);
    }
}

void linear_motion::unlock()
{
    auto command = axis_cmds.find(0);
    if (command != axis_cmds.end()) {
        std::cout << "sending command: " << command->second << '\n';
        sendDirectCmd(command->second);
    }

}