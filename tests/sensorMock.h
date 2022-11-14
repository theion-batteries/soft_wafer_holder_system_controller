#pragma once 

#include "Idistance_sensor.h"

class sensorMock: public Idistance_sensor
{
private:
    double currentVal = 303;
public:
    sensorMock(/* args */);
    virtual  ~sensorMock();
    virtual double getMesuredValue()override;
    virtual wgm_feedbacks::enum_sub_sys_feedback connect() override;
    virtual void disconnect() override;
    virtual bool getStatus() override;




};