/**
 * @file whs_controller.cpp
 * @author your name (you@domain.com)
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
    std::cout << "loading config file" << std::endl;
    std::ifstream filein(WHS_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(WHS_CONFIG);
    _whs_params.fi = (config["script"].as<std::string>());
    _whs_params.py = (config["interpreter"].as<std::string>());
    _whs_params.cmd = _whs_params.py + " " + _whs_params.fi;
    _whs_params.pyCmd = &_whs_params.cmd[0];
    _whs_params.pyFile = &_whs_params.fi[0];
    _whs_params.pyInterpreter = _whs_params.py.c_str();
    _whs_params.mm_steps = config["mm_steps"].as<double>();
    _whs_params.mm_step_res = config["mm_step_res"].as<double>();
    _whs_params.ref_dis = config["ref_dis"].as<double>();
    _whs_params.delay_to_move_request = config["delay_to_move_request"].as<DWORD>();
    _whs_params.thickness = config["thickness"].as<double>();

#endif 
    distSensor = std::make_shared< keyence_sensor>();
    linearMover = std::make_shared< linear_motion>();
}
/**
 * @brief Destroy the whs controller::whs controller object
 *
 */
whs_controller::~whs_controller()
{
}

void whs_controller::reload_config_file()
{
    std::cout << "reloading config file" << std::endl;
    std::ifstream filein(WHS_CONFIG);
    for (std::string line; std::getline(filein, line); )
    {
        std::cout << line << std::endl;
    }
    config = YAML::LoadFile(WHS_CONFIG);
    _whs_params.fi = (config["script"].as<std::string>());
    _whs_params.py = (config["interpreter"].as<std::string>());
    _whs_params.cmd = _whs_params.py + " " + _whs_params.fi;
    _whs_params.pyCmd = &_whs_params.cmd[0];
    _whs_params.pyFile = &_whs_params.fi[0];
    _whs_params.pyInterpreter = _whs_params.py.c_str();
    _whs_params.mm_steps = config["mm_steps"].as<double>();
    _whs_params.mm_step_res = config["mm_step_res"].as<double>();
    _whs_params.ref_dis = config["ref_dis"].as<double>();
    _whs_params.delay_to_move_request = config["delay_to_move_request"].as<DWORD>();
    _whs_params.thickness = config["thickness"].as<double>();

}
/**************** Algorithms conntroller ***************/


void whs_controller::move_down_until_data_availble()
{
    current_axis_position = linearMover->get_position();

    std::cout << "start pos:  " << current_axis_position << std::endl;
    while (distSensor->getMesuredValue() == 0) // while data invalid, we go down further
    {
        std::cout << "moving down by " << _whs_params.mm_steps << "mm_steps until reading values " << std::endl;
        linearMover->move_down_by(_whs_params.mm_steps); // move down by mm steps
    }
}
/**
 * @brief
 *
 * @param ref_dis
 */
void whs_controller::move_down_to_surface()
{
    std::cout << "moving down to surface" << std::endl;
    std::cout << "moving down until sensor reading is equal the refernce distance: " << _whs_params.ref_dis << std::endl;
    while (distSensor->getMesuredValue() >= _whs_params.ref_dis) // while distSensor reading is <= to reference distance
    {
        std::cout << "moving down by " << _whs_params.one_mm_steps << "mm_steps until reading values " << std::endl;
        linearMover->move_down_by(_whs_params.one_mm_steps); // move down by mm steps
        std::cout << "current pos:  " << linearMover->get_position() << std::endl;
    }
}
/**
 * @brief
 *
 * @param thickness
 * @param mm_step_res
 */
void whs_controller::deep_wafer_holder_desired_thickness() //default to 0.01 mm_step x 10 steps= 0.1mm or 100µm
{

    auto steps = _whs_params.thickness / _whs_params.mm_step_res;
    for (unsigned int step_counter = 0; step_counter < steps; step_counter++)
    {
        std::cout << "iteration number " << step_counter << std::endl;
        linearMover->move_down_by(_whs_params.mm_step_res); // move mm step default 0.01 mm
        // to check
        distSensor->getMesuredValue();
        // to verify step res
        linearMover->get_position();
    }
    waferHolderReady = true;
}
/**
 * @brief function to monitor the distance and calibrate depth
 */
void whs_controller::monitor_and_calibrate()
{
    std::cout << "monitor calibration started" << std::endl;
    // algorithm: if current distance is not equal desired thickness, calibrate
    // 1. check if diff is a value that is below the step resolution, continue or
    // 2. move up if diff negativ
    // 3. move down if diff positiv
    while (true)
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
                linearMover->move_up_by(diff);
            }
            else
            {
                linearMover->move_down_by(diff);

            }

        }

    }


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
Idistance_sensor*  whs_controller::get_dist_ptr()
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
    //distSensor->sendDirectCmd(cmd);

}
void whs_controller::sendDirectCmdAxis(std::string& cmd)
{
linearMover->sendDirectCmd(cmd);
}