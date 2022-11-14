#include "sensorMock.h"


sensorMock::sensorMock(/* args */)
{
        std::cout << "creating keyence mock" <<std::endl;

}

sensorMock::~sensorMock()
{

}
bool sensorMock::getStatus()
{
    return true;
}


void sensorMock::disconnect() 
{
    std::cout<<"disconnting"<<std::endl;
}
double sensorMock::getMesuredValue()
{

    return currentVal--;    
}


/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback sensorMock::connect()
{
    std::cout << "connect keyence mock " << std::endl;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
}
