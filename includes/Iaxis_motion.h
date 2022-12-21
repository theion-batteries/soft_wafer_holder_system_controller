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
#include <iostream>
#include "subsystem_feedback.h"
class Iaxis_motion
{
private:
    /* data */
public:
    Iaxis_motion(/* args */);
    virtual ~Iaxis_motion();
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() = 0;
    virtual void disconnect() = 0;
    virtual void move_home() = 0;
    virtual void move_to(int new_position) = 0;
    void sendCmd(std::string& cmd, sockpp::tcp_connector* client, std::string args = std::string());
    virtual std::string sendDirectCmd(std::string cmd)=0;
    virtual double get_position() = 0;
    virtual bool getStatus() =0;
    virtual void get_speed() =0;
    virtual void set_speed(double_t new_val) =0;
    virtual void move_up_to(double_t new_pos) =0;
    virtual void move_down_to(double_t new_pos) =0;
    virtual void move_up_by(double_t steps) =0;
    virtual void move_down_by(double_t steps) =0;
};



