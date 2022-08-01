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
#pragma once
/**
 * @brief responsible for executing subprocess, handle data in both directions 
 * and process the data with developped algorithms
 * 
 */
struct keyence_server
{
    std::string ip="127.0.0.1";
    std::string port="5555";
};
struct delta_server
{
    std::string ip="127.0.0.1";
    std::string port="5550";
};

class whs_controller
{
private:
	LPCWSTR pyFilePath = L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/software_repetier_rest_api/src/repetier_manager_lib/repetier_manager.py";
	LPCWSTR pyCmd = L"C:/Users/SamiDhiab/AppData/Local/Programs/Python/Python39/python.exe";
	LPCWSTR cppFile =L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/lib_keyence_distance_sensor/build/keyence_bin.exe";
    delta_server* _delta_struct;
    keyence_server* _keyence_struct;

    /* data */
public:
    whs_controller(/* args */);
    void run_delta();
    void run_keyence();
    void run_both();
    whs_controller(delta_server* delta, keyence_server* keyence);



    ~whs_controller();
};
