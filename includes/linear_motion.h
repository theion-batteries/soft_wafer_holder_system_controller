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

#include "Iaxis_motion.h"

struct whs_axis_motion_server
{
    const char* ip = "192.168.0.100";
    uint16_t port = 80;
};

class linear_motion: public Iaxis_motion
{
private:
    /* data */
public:
    linear_motion(/* args */);
    virtual ~linear_motion();
    virtual void move_home() override;
    virtual void move_to(int new_position) override;


};
















