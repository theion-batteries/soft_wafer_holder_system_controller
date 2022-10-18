/**
 * @file wafer_holder_motion_system.h
 * @author sami dhiab (sami@theion.de)
 * @brief wafer motion system package
 * @version 0.1
 * @date 2022-07-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>
#include "whs_controller.h"
#include "yaml-cpp/yaml.h"
#include <memory>


enum class enum_sys_feedback
{
    sys_success = 2,
    sys_error = -1
};
namespace wafer_holder_motion_system
{
    /****** delta motion ******/
    // interface
    class Idelta_motion
    {
    public:
        virtual void move(int direction) = 0;

        Idelta_motion();

        virtual ~Idelta_motion();
    };
    // implementation
    class delta_motion :public Idelta_motion
    {
    public:
        explicit delta_motion(std::shared_ptr<whs_controller> shared_controller);

    protected:

        virtual void move(int direction);
    private:
        std::shared_ptr<whs_controller>   wafer_delta_shared_ptr;

    };

    /****** distance sensor******/
    // interface    
    class Idistance_sensor
    {
    public:
        Idistance_sensor();
        virtual ~Idistance_sensor();
        virtual double read_values() = 0;
    };
    // implementation
    class distance_sensor :public Idistance_sensor
    {
    public:
        explicit distance_sensor(std::shared_ptr<whs_controller> shared_controller);
    protected:
        virtual double read_values();
    private:
        std::shared_ptr<whs_controller>   wafer_dist_shared_ptr;

    };

    /****** wafer motion controller ******/
    // interface    
    class Iwafer_motion_controller
    {
    public:
        Iwafer_motion_controller();
        virtual ~Iwafer_motion_controller();
        virtual void move_down() = 0;
        virtual void calibrate() = 0;
        virtual void move_up() = 0;
        virtual double get_current_value(uint16_t sensor_head) = 0;
        virtual void insert_wafer_in_ml() = 0;
        virtual void extract_wafer_from_ml() = 0;
        virtual void set_distance_to_surface_contact(double distance) = 0;
    };
    // implementation
    class wafer_motion_controller :public Iwafer_motion_controller
    {
    public:
        explicit wafer_motion_controller();
        virtual ~wafer_motion_controller();
    protected:
        virtual void move_down();
        virtual void calibrate();
        virtual void move_up();
        virtual double get_current_value(uint16_t sensor_head);
        virtual void insert_wafer_in_ml();
        virtual void extract_wafer_from_ml();
        virtual void set_distance_to_surface_contact(double distance);

    private:
        //Idelta_motion* delta_mover;
        //Idistance_sensor* dist_sensor;
        std::shared_ptr<whs_controller> wafer_sys_control_shared_ptr;// = std::make_shared<whs_controller>();
        YAML::Node config = YAML::LoadFile("./config.yaml");
        double distance_to_surface_contact; // distance where wafer holder extact with ML surface
        double distance_to_slow_down = 30; //30mm=3cm: distance from where the speed of delta motion slow down
    };

}

