/**
 * @file Iaxis_motion.h
 * @author sami dhiab
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Iaxis_motion.h"
Iaxis_motion::Iaxis_motion(/* args */)
{
}

Iaxis_motion::~Iaxis_motion()
{
}



/**
 * @brief
 *
 * @param cmd
 * @param client
 * @param args
 */
void Iaxis_motion::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}

