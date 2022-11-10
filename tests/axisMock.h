#pragma once 
#include "linear_motion.h"

class axisMock : public linear_motion
{
private:
    /* data */
public:
    axisMock(/* args */);
    virtual ~axisMock();
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() override;
    virtual void disconnect() override;
    virtual void move_home() override;
    virtual void move_to(int new_position) override;
    virtual std::string sendDirectCmd(std::string& cmd)override;
    virtual double get_position() override;
    virtual bool getStatus() override;
    virtual void get_speed() override;
    virtual void set_speed(double_t new_val) override;
    virtual void move_up_to(double_t new_pos) override;
    virtual void move_down_to(double_t new_pos) override;
    virtual void move_up_by(double_t steps) override;
    virtual void move_down_by(double_t steps) override;
    void waitForResponse();
    void move_center();
    void unlock();
};



