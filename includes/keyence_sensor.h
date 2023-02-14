/**
 * @file keyence_sensor.h
 * @author sami dhiab sami@theion.de
 * @brief
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include "Idistance_sensor.h"
#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <queue>
#include <keyence_client.h>
#include <thread>
#include <atomic>
#include <filesystem>
#include "yaml-cpp/yaml.h"
struct keyence_server
{
    std::string ip = "192.168.0.104";
    uint16_t port = 6555;
};

class keyence_sensor: public Idistance_sensor
{
private:
    keyence_server _keyence_struct;
    keyence_client* Kclient;
    std::map<u_int, std::string> keyence_cmds = {
        {1,"get1"}, {2,"get2"},
        {3,"get3"}, {4,"get_all"},
        {5,"set_R0"}, {6,"set_Q0"},
    };
    std::string keyence_incoming_data;
    u_int keyence_data_length = 1024;
    bool keyenceReady = false;
    std::deque<double> keyence_last_mesured; // FIFO last 10 values
    std::deque<double> keyence_last_mesured_output0; // FIFO last 10 values
    std::deque<double> keyence_last_mesured_output1; // FIFO last 10 values
    std::deque<double> keyence_last_mesured_output2; // FIFO last 10 values

public:
    keyence_sensor(/* args */);
    keyence_sensor(std::string ip, uint16_t port);
    ~keyence_sensor();
    virtual double getMesuredValue()override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    // keyence methods
    double keyence_client_get_value_output0();
    double keyence_client_get_value_output1();
    double keyence_client_get_value_output2();
    double keyence_client_get_value_output_all() override;
    virtual bool getStatus() override;
};

