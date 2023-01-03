/**
   @file whs_controller.h
   @author sami dhiab (sami@theion.de)
   @brief controller wafer holder system
   @version 0.1
   @date 2022-08-01

   @copyright Copyright (c) 2022

*/
#pragma once
#include "sockpp/socket.h"
#include <iostream>
#include <string>
#include "sockpp/tcp_connector.h"
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <queue>
#include "memory.h"
#include <keyence_client.h>
#include <thread>
#include <future>
#include <atomic>
#include <filesystem>
#include "yaml-cpp/yaml.h"

//#define MOCK_SENSOR
#ifdef SINK_SENSOR_MOCK
#include "sensorMock.h"
#else
#include "keyence_sensor.h"
#endif
#ifdef SINK_AXIS_MOCK
#include "axisMock.h"
#else
#include "linear_motion.h"
#endif

struct whs_config_yaml_params
{
    LPCTSTR pyInterpreter;
    LPSTR pyFile;
    LPSTR pyCmd;
    std::string py;
    std::string fi;
    std::string cmd;
    double mm_steps; // distance to move down  default 10 mm
    DWORD delay_to_move_request; // wait between move request  default 2000ms
    double ref_dis;// parameter calibration sensor  default 
    double thickness; // thickness of wafer holder default 0.1mm
    double mm_step_res;
    int one_mm_steps = 1;
    int MaxSafePos = 250;

};

class whs_controller
{
public:
     YAML::Node config;
     whs_config_yaml_params _whs_params;
private:
    bool waferHolderReady = false;
    std::shared_ptr< Idistance_sensor> distSensor;
    std::shared_ptr <Iaxis_motion> linearMover;
    double current_axis_position;
    double current_sensor_value;
public:
    /******* const/desctr ****/
    whs_controller();
    ~whs_controller();
    /******** algorithms controller **********/

    void move_down_until_data_availble();
    void move_down_to_surface();
    void deep_wafer_holder_desired_thickness(); //default to 0.01 mm_step x 10 steps= 0.1mm or 100µm
    void monitor_and_calibrate(); // we will launch a monitor to check if we are inside the desired depth during next processes
    /********* helper functions */
    bool get_mover_status();
    bool get_sensor_status();
    bool get_whs_controller_status();
    /*     helper getter */
    double get_sensor_values();
    double get_axis_position();
    Iaxis_motion* get_axis_ptr();
    Idistance_sensor* get_dist_ptr();
    void sendDirectCmdSensor(std::string& cmd);
    std::string sendDirectCmdAxis(std::string cmd);
     void reload_config_file();

};
