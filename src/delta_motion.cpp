/**
 * @file Iaxis_motion.h
 * @author sami dhiab
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "delta_motion.h"

delta_motion::delta_motion(LPCTSTR pyInterp, LPSTR pyCmdFull)
{
    std::cout << "creating delta client" << std::endl;
    pyInterpreter = pyInterp; pyCmd = pyCmdFull;
}

delta_motion::~delta_motion()
{
    if (delta_client_sock != nullptr) delete delta_client_sock;
}

bool delta_motion::getStatus()
{
    return deltaReady;
}

std::string delta_motion::sendDirectCmd(std::string cmd)
{
    if (delta_client_sock->write(cmd) != ssize_t(std::string(cmd).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << delta_client_sock->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd << " sent" << std::endl;
    return "";
}

/**
 * @brief TODO: change the function call
 *
 * @param new_position
 */
void delta_motion::move_to(int new_position)
{

    move_up_by(new_position);

}

wgm_feedbacks::enum_sub_sys_feedback delta_motion::connect()
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
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    std::cout << "Created a connection from " << delta_client_sock->address() << std::endl;
    std::cout << "Created a connection to " << delta_client_sock->peer_address() << std::endl;
    // Set a timeout for the responses
    if (!delta_client_sock->read_timeout(std::chrono::seconds(5))) {
        std::cerr << "Error setting timeout on TCP stream: "
            << delta_client_sock->last_error_str() << std::endl;
        deltaReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
    deltaReady = true;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

void delta_motion::disconnect()
{
    delta_client_sock->close();
}


/**
 * @brief
 *
 */
void delta_motion::run_delta_subprocess() {
    std::cout << "Running delta program in new console" << std::endl;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // Start the child process. 
    if (!CreateProcess(pyInterpreter,   // No module name (use command line)
        pyCmd,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, NULL);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}

/**
 * @brief
 *
 * @return double
 */
double delta_motion::get_position()
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
        std::cout << "n bytes: " << n << std::endl;
        std::cout << "cmd len: " << ssize_t(command->second.length()) << std::endl;
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
void delta_motion::move_home()
{
    double last_pos = 0;
    if (!delta_last_position.empty())
    {
        last_pos = delta_last_position.front(); // this shall start at 300
        if (last_pos == 300) return; //already
    }
    else {
        last_pos = get_position();
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
void delta_motion::get_speed()
{

}
void delta_motion::set_speed(double_t new_val)
{

}
void delta_motion::move_up_to(double_t new_pos)
{

}
void delta_motion::move_down_to(double_t new_pos)
{

}
/**
 * @brief
 *
 * @param steps
 */
void delta_motion::move_up_by(double_t steps)
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
void delta_motion::move_down_by(double_t steps)
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
