/**
   @file whs_controller.h
   @author sami dhiab (sami@theion.de)
   @brief controller wafer holder system
   @version 0.1
   @date 2022-08-01

   @copyright Copyright (c) 2022

*/
#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <queue>
#include "keyence_client.h"
#pragma once
/**
 * @brief responsible for executing subprocess, handle data in both directions 
 * and process the data with developped algorithms
 * 
 */
struct keyence_server
{
    const char* ip="127.0.0.1";
    uint16_t port=6555;
};
struct delta_server
{
    const char* ip="127.0.0.1";
    uint16_t port=6550;
};

class whs_controller
{
private:
	LPCWSTR pyFilePath = L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/software_repetier_rest_api/src/repetier_manager_lib/delta_server.py";
	LPCWSTR pyCmd = L"C:/Users/SamiDhiab/AppData/Local/Programs/Python/Python39/python.exe";
	LPCWSTR cppFile =L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/lib_keyence_distance_sensor/build/Debug/keyence_bin.exe";
    delta_server _delta_struct;
    keyence_server _keyence_struct;
    sockpp::socket_initializer sockInit;
    sockpp::tcp_connector* delta_client_sock;
    sockpp::tcp_connector* keyence_client_sock;
    const char* keyence_ip = "192.168.0.104";
    keyence_client* Kclient;
    std::map<u_int, std::string> keyence_cmds={
        {1,"get1"}, {2,"get2"}, 
        {3,"get3"}, {4,"get_all"}, 
        {5,"set_R0"}, {6,"set_Q0"}, 
    };
    std::map<u_int, std::string> delta_cmds={
        {1,"get_pos"}, {2,"get_speed"}, 
        {3,"up_to"}, {4,"down_to"}, 
        {5,"up_by"}, {6,"down_by"}, 
    };
    std::deque<double> keyence_last_mesured; // FIFO last 10 values
    std::string keyence_incoming_data;
    u_int keyence_data_length = 1024;
    std::string delta_incoming_data;
    u_int delta_data_length = 1024;
public:
    /******* const/desctr ****/
    whs_controller(/* args */);
    ~whs_controller();
    /******* client controller methods ***/
    void close_all_sockets();
    void run_delta_subprocess();
    void run_keyence_subprocess();
    void keyence_client_connect();
    void run_all_subprocesses();
    void connect_to_delta_server();
    void connect_to_keyence_server();
    void sendCmd(std::string cmd, sockpp::tcp_connector* client);
    void get_keyence_sensor_mesured_Values();
    void set_keyence_mesurement_mode();
    void get_delta_position();
    void get_delta_speed();
    void set_delta_speed(double_t new_val);
    void move_delta_up_to(double_t new_pos);
    void move_delta_down_to(double_t new_pos);
    void move_delta_up_by(double_t steps);
    void move_delta_down_by(double_t steps);

};
