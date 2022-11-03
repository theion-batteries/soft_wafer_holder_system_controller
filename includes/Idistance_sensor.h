/**
 * @file Idistance_sensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include <iostream>
#include "subsystem_feedback.h"
class Idistance_sensor
{
private:
    /* data */
public:
    Idistance_sensor(/* args */);
    virtual  ~Idistance_sensor();
    virtual double getMesuredValue()=0;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    virtual void disconnect() = 0;
    virtual void sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args = std::string());
    virtual bool getStatus() =0;

};

