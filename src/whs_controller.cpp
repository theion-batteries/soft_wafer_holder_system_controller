/**
 * @file whs_controller.cpp
 * @author sami dhiab sami@theion.de
 * @brief
 * @version 0.1
 * @date 2022-08-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "whs_controller.h"
#include <fstream>


whs_controller::whs_controller()
{
    std::cout << "creating subsystem wafer holder motion controller " << std::endl;
#ifdef WHS_CONFIG
    std::cout << "loading config file: " << WHS_CONFIG << std::endl;
    std::ifstream filein(WHS_CONFIG);
    //for (std::string line; std::getline(filein, line); )
    //{
    //    std::cout << line << std::endl;
    //}
    config = YAML::LoadFile(WHS_CONFIG);


    _whs_params.mm_steps = config["mm_steps"].as<double>();
    _whs_params.mm_step_res = config["mm_step_res"].as<double>();
    _whs_params.ref_dis = config["ref_dis"].as<double>();
    _whs_params.delay_to_move_request = config["delay_to_move_request"].as<DWORD>();
    _whs_params.thickness = config["thickness"].as<double>();
    _whs_params.MaxSafePos = config["MaxSafePos"].as<int>();
    _whs_params.wafer_travel = config["wafer_travel"].as<double>();
    _whs_params.wafer_max_speed = config["wafer_max_speed"].as<double>();
    _whs_params.motion_server_ip = config["motion_server_ip"].as<std::string>(); 
    _whs_params.motion_server_port = config["motion_server_port"].as<uint16_t>();
    _whs_params.distance_sensor_server_ip = config["distance_sensor_server_ip"].as<std::string>();
    _whs_params.distance_sensor_server_port = config["distance_sensor_server_port"].as<uint16_t>();

    std::cout << "axis server ip:  " << _whs_params.motion_server_ip << std::endl;
    std::cout << "keyence server ip:  " << _whs_params.distance_sensor_server_ip << std::endl;

#endif 
#ifdef SINK_SENSOR_MOCK
    distSensor = std::make_shared< sensorMock>();
#else
    distSensor = std::make_shared< keyence_sensor>(_whs_params.distance_sensor_server_ip, _whs_params.distance_sensor_server_port);
#endif
#ifdef SINK_AXIS_MOCK
    linearMover = std::make_shared< axisMock>();
#else
    linearMover = std::make_shared< linear_motion>(_whs_params.motion_server_ip, _whs_params.motion_server_port);
#endif
}
/**
 * @brief Destroy the whs controller::whs controller object
 *
 */
whs_controller::~whs_controller()
{
}

wgm_feedbacks::enum_sub_sys_feedback whs_controller::reload_config_file()
{
    std::cout << "reloading config file: " << WHS_CONFIG << std::endl;
    std::ifstream filein(WHS_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(WHS_CONFIG);

    _whs_params.mm_steps = config["mm_steps"].as<double>();
    _whs_params.mm_step_res = config["mm_step_res"].as<double>();
    _whs_params.ref_dis = config["ref_dis"].as<double>();
    _whs_params.delay_to_move_request = config["delay_to_move_request"].as<DWORD>();
    _whs_params.thickness = config["thickness"].as<double>();
    _whs_params.MaxSafePos = config["MaxSafePos"].as<int>();
    _whs_params.wafer_travel = config["wafer_travel"].as<double>();
    _whs_params.wafer_max_speed = config["wafer_max_speed"].as<double>();
    _whs_params.motion_server_ip = config["motion_server_ip"].as<std::string>().c_str();
    _whs_params.motion_server_port = config["motion_server_port"].as<uint16_t>();
    _whs_params.distance_sensor_server_ip = config["distance_sensor_server_ip"].as<std::string>().c_str();
    _whs_params.distance_sensor_server_port = config["distance_sensor_server_port"].as<uint16_t>();

    return sub_success;

}

wgm_feedbacks::enum_sub_sys_feedback whs_controller::reset_config_file() // set config file params to default
{

    std::cout << "resetting config file: " << WHS_CONFIG << std::endl;
    config = YAML::LoadFile(WHS_CONFIG);

    config["mm_steps"] = _whs_params_default.mm_steps;
    config["mm_step_res"] = _whs_params_default.mm_step_res;
    config["ref_dis"] = _whs_params_default.ref_dis;
    config["delay_to_move_request"] = _whs_params_default.delay_to_move_request;
    config["thickness"] = _whs_params_default.thickness;
    config["MaxSafePos"] = _whs_params_default.MaxSafePos;
    config["wafer_travel"] = _whs_params_default.wafer_travel;
    config["wafer_max_speed"] = _whs_params_default.wafer_max_speed;
    config["motion_server_ip"] = _whs_params.motion_server_ip;
    config["motion_server_port"] = _whs_params.motion_server_port;
    config["distance_sensor_server_ip"] = _whs_params.distance_sensor_server_ip;
    config["distance_sensor_server_port"] = _whs_params.distance_sensor_server_port;

    std::ofstream fout(WHS_CONFIG);
    fout << config;
    fout.close();
    std::ifstream filein(WHS_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    return sub_success;

}

wgm_feedbacks::enum_sub_sys_feedback whs_controller::open_config_file()
{
    std::string file = WHS_CONFIG;
    std::cout << "opening config file in notepad \n";
    std::string command = "notepad.exe " + file;
    auto val = system(command.c_str());
    if (val == 0) return sub_success;
    return sub_error;
}


/**************** Algorithms conntroller ***************/
wgm_feedbacks::enum_sub_sys_feedback whs_controller::connect_controller()
{
    if (distSensor->connect() == sub_error || linearMover->connect() == sub_error) return sub_error;
    waferHolderReady = true;
    return sub_success;

}

wgm_feedbacks::enum_sub_sys_feedback whs_controller::disconnect_controller()
{
    if (distSensor->disconnect() == sub_error || linearMover->disconnect() == sub_error) return sub_error;
    waferHolderReady = false;
    return sub_success;
}

wgm_feedbacks::enum_sub_sys_feedback whs_controller::move_down_until_data_availble()
{

 //   if (!waferHolderReady) return sub_error;
        std::cout << "moving home" << std::endl;

    if (linearMover->move_home() == sub_error) return sub_error;

    std::cout << "<----------------------------------------------> " << std::endl;
    std::cout << "algorithm move down until data valid launched" << std::endl;
    if (!waferHolderReady) return sub_error;
    if (linearMover->move_home() == sub_error) return sub_error;
    current_axis_position = linearMover->get_position();

    std::cout << "axis start pos:  " << current_axis_position << std::endl;

    while (distSensor->getMesuredValue() == 0) // while data invalid, we go down further
    {
        // algo1: given max trajectory 
        if (_whs_params.MaxSafePos > _whs_params.mm_steps)
        {
            std::cout << "moving all way down to " << _whs_params.MaxSafePos << "mm_steps until reading values " << std::endl;
            linearMover->move_down_to(_whs_params.MaxSafePos); // move down to max
        }
        // algo2: move long steps
        else
        {
            std::cout << "moving down by " << _whs_params.mm_steps << "mm_steps until reading values " << std::endl;
            linearMover->move_down_to(abs(current_axis_position - _whs_params.mm_steps)); // move down by mm steps
            current_axis_position = current_axis_position - _whs_params.mm_steps;
            std::cout << "new axis position " << current_axis_position << std::endl;
        }

    }
    std::cout << "sensor value: " << distSensor->getMesuredValue() << std::endl;
    std::cout << "sensor data valid" << std::endl;
    std::cout << "algorithm finished succeffuly " << std::endl;
    std::cout << "<----------------------------------------------> " << std::endl;
    return sub_success;


}
/**
 * @brief
 *
 * @param ref_dis
 */
wgm_feedbacks::enum_sub_sys_feedback whs_controller::move_down_to_surface()
{

    if (!waferHolderReady) return sub_error;

    std::cout << "<----------------------------------------------> " << std::endl;
    std::cout << "algorithm surface contact launched" << std::endl;

    std::cout << "moving down to surface" << std::endl;
    current_axis_position = linearMover->get_position();
    std::cout << "current pos:  " << current_axis_position << std::endl;

    while (distSensor->getMesuredValue() >= _whs_params.ref_dis) // while distSensor reading is <= to reference distance
    {
        std::cout << "moving down until sensor reading is equal the refernce distance: " << _whs_params.ref_dis << std::endl;
        std::cout << "moving down by " << _whs_params.one_mm_steps << std::endl;
        linearMover->move_down_to(abs(current_axis_position - _whs_params.one_mm_steps)); // move down by mm steps
        current_axis_position = current_axis_position - _whs_params.one_mm_steps;
        std::cout << "new axis position " << current_axis_position << std::endl;
    }
    std::cout << "sensor value: " << distSensor->getMesuredValue() << std::endl;
    std::cout << "algorithm finished succeffuly " << std::endl;
    std::cout << "<----------------------------------------------> " << std::endl;
    return sub_success;

}
/*
 * @brief
 *
 * @param thickness
 * @param mm_step_res
 */
wgm_feedbacks::enum_sub_sys_feedback whs_controller::deep_wafer_holder_desired_thickness() //default to 0.01 mm_step x 10 steps= 0.1mm or 100µm
{
    std::cout << "algorithm deeping launched " << std::endl;
    std::cout << "<----------------------------------------------> " << std::endl;
    auto steps = _whs_params.thickness / _whs_params.mm_step_res;
    current_axis_position = get_axis_position();
    for (unsigned int step_counter = 0; step_counter < steps; step_counter++)
    {
        std::cout << "iteration number " << step_counter << std::endl;
        linearMover->move_down_to(abs(current_axis_position - _whs_params.mm_step_res)); // move mm step default 0.01 mm
        current_axis_position -= _whs_params.mm_step_res;
        distSensor->getMesuredValue();
        linearMover->get_position();
    }
    distSensor->getMesuredValue();
    linearMover->get_position();
    waferHolderReady = true;
    // TODO: on success, calibrate is true
    // calibrate = true;
    std::cout << "algorithm finished succeffuly " << std::endl;
    std::cout << "<----------------------------------------------> " << std::endl;
    return sub_success;

}
/**
 * @brief function to monitor the distance and calibrate depth
 */
wgm_feedbacks::enum_sub_sys_feedback whs_controller::monitor_and_calibrate()
{
    std::cout << "monitor calibration started" << std::endl;
    // algorithm: if current distance is not equal desired thickness, calibrate
    // 1. check if diff is a value that is below the step resolution, continue or
    // 2. move up if diff negativ
    // 3. move down if diff positiv
    while (calibrate) // TODO: add calibrate variable
    {
        double diff = distSensor->getMesuredValue() - _whs_params.thickness;
        if (abs(diff) <= _whs_params.mm_step_res) // if the difference mesured lower than min step res, we skip calibration
        {
            continue;
        }
        else
        {
            if (diff < 0) // if diff negativ, we move up
            {
                linearMover->move_up_by(abs(diff));
            }
            else
            {
                linearMover->move_down_by(abs(diff));

            }

        }

    }
    return sub_success;



}
wgm_feedbacks::enum_sub_sys_feedback whs_controller::extract_move_home()
{
    return linearMover->move_home();
}


/********* helper functions */
bool whs_controller::get_mover_status()
{
    return linearMover->getStatus();
}
bool whs_controller::get_sensor_status()
{
    return distSensor->getStatus();
}
bool whs_controller::get_whs_controller_status()
{
    return waferHolderReady;
}

/*                getter interface              */

double whs_controller::get_sensor_values()
{

    return distSensor->getMesuredValue();
}
double whs_controller::get_axis_position()
{

    return linearMover->get_position();
}

Iaxis_motion* whs_controller::get_axis_ptr()
{
    return dynamic_cast<Iaxis_motion*>(linearMover.get());
}
Idistance_sensor* whs_controller::get_dist_ptr()
{
    return dynamic_cast<Idistance_sensor*>(distSensor.get());
}
/**
 * @brief TODO implement direct send cmd to sensor if needed
 *
 * @param cmd
 */
void whs_controller::sendDirectCmdSensor(std::string& cmd)
{
    // distSensor->sendDirectCmd(cmd);
}
std::string whs_controller::sendDirectCmdAxis(std::string cmd)
{
    return  linearMover->sendDirectCmd(cmd);
}

