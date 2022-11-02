
#include "Idistance_sensor.h"


Idistance_sensor::Idistance_sensor(/* args */)
{
}

Idistance_sensor::~Idistance_sensor()
{
}


/**
 * @brief
 *
 * @param cmd
 * @param client
 * @param args
 */
void Idistance_sensor::sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args)
{
    if (client->write(cmd + args) != ssize_t(std::string(cmd + args).length())) {
        std::cerr << "Error writing to the TCP stream: "
            << client->last_error_str() << std::endl;
    }
    std::cout << "command " << cmd + args << " sent" << std::endl;
}