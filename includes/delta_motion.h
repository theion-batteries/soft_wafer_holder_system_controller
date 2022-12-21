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
#include <queue>
#include <keyence_client.h>
#include <thread>
#include <atomic>
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "Iaxis_motion.h"
struct delta_server
{
    const char* ip = "127.0.0.1";
    uint16_t port = 6550;
};

class delta_motion : public Iaxis_motion
{
private:
    delta_server _delta_struct;
    sockpp::socket_initializer sockInit;
    sockpp::tcp_connector* delta_client_sock=nullptr;
    bool deltaReady = false;
    LPCTSTR pyInterpreter;
    LPSTR pyFile;
    LPSTR pyCmd;

    std::map<u_int, std::string> delta_cmds = {
        {1,"get_pos"}, {2,"get_speed"},
        {3,"move_to_z"}, {4,"move_to_xyz"},
        {5,"move_up_by"}, {6,"move_down_by"},
        {7,"move_home"}
    };
    std::deque<double> delta_last_position; // FIFO last 10 values
    std::string delta_incoming_data;
    u_int delta_data_length = 1024;

public:
    delta_motion(LPCTSTR pyInterp, LPSTR pyCmdFull);
    virtual ~delta_motion();
    void move_home() override;
    void move_to(int new_position) override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect()override;
    virtual void disconnect() override;
    void run_delta_subprocess();
    double get_position() override;
    void get_speed() override;
    void set_speed(double_t new_val) override;
    void move_up_to(double_t new_pos) override;
    void move_down_to(double_t new_pos) override;
    void move_up_by(double_t steps) override;
    void move_down_by(double_t steps) override;
    virtual bool getStatus() override;
    virtual std::string sendDirectCmd(std::string cmd) override;

};

















