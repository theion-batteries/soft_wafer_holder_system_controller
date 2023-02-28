/**
 * @file Iaxis_motion.h
 * @author sami dhiab
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
#include "Windows.h" 
#include <shellapi.h> 
#include <map>
#include <thread>
#include <atomic>
#include <queue>
#include <future>
#include <memory>
#include "Iaxis_motion.h"
#include <string>

struct whs_axis_motion_server
{
    std::string ip = "192.168.0.209";
    uint16_t port = 8882;
    double max_travel = 130;
    double wafer_travel = 100 ;       //# used --> # should be determined using the keyence sensor in the future
    double wafer_max_speed = 800  ;   //# used
    uint16_t timeout = 10;

};



class linear_motion: public Iaxis_motion
{
private:
    whs_axis_motion_server _motion_axis_struct;
    sockpp::socket_initializer sockInit;
    sockpp::tcp_connector* _client = nullptr;
    bool axisReady = false;
    std::map<std::string, std::string> axis_cmds = {
        {"unlock","$X"}, {"get_position","?"}, {"move","X"},
        {"get_setting","$$"}, {"set_speed","$110="}, {"home","$H"},
        {"pause","!"}, {"resume","~"}
    };
    std::deque<double> axis_last_position; // FIFO last 10 values
    std::string incoming_data;
    size_t axis_data_length = 5012;
public:
    linear_motion(std::string ip, uint16_t port, const uint16_t timeout);
    virtual ~linear_motion();
    virtual wgm_feedbacks::enum_sub_sys_feedback move_home() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback move_to(int new_position) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect()override;
    virtual wgm_feedbacks::enum_sub_sys_feedback disconnect() override;
    wgm_feedbacks::enum_sub_sys_feedback set_center_position(double new_target) ;
    double get_position() override;
    double get_speed() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback pause() override;
    virtual wgm_feedbacks::enum_sub_sys_feedback resume() override;
    virtual std::string get_settings() override;
    wgm_feedbacks::enum_sub_sys_feedback set_speed(double_t new_val) override;
    wgm_feedbacks::enum_sub_sys_feedback move_up_to(double_t new_pos) override;
    wgm_feedbacks::enum_sub_sys_feedback move_down_to(double_t new_pos) override;
    wgm_feedbacks::enum_sub_sys_feedback move_up_by(double_t steps) override;
    wgm_feedbacks::enum_sub_sys_feedback move_down_by(double_t steps) override;
    virtual bool getStatus() override;
    virtual std::string sendDirectCmd(std::string cmd) override;
    std::string waitForResponse();
    wgm_feedbacks::enum_sub_sys_feedback move_center();
    wgm_feedbacks::enum_sub_sys_feedback unlock();
};

















