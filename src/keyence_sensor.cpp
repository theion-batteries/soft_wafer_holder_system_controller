#include "keyence_sensor.h"


keyence_sensor::keyence_sensor(/* args */)
{
        std::cout << "creating keyence client" <<std::endl;

}

keyence_sensor::~keyence_sensor()
{
    delete Kclient;
}
bool keyence_sensor::getStatus()
{
    return keyenceReady;
}


void keyence_sensor::disconnect() 
{
    Kclient->disconnect();
}
double keyence_sensor::getMesuredValue()
{

    return keyence_client_get_value_output0();    
}
/************************* keyence client methods*****************/
/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback keyence_sensor::connect()
{
    std::cout << "Running keyence client " << std::endl;
    Kclient = new keyence_client(keyence_ip);
    wgm_feedbacks::enum_hw_feedback Keyence_feedback = Kclient->connect();
    if (Keyence_feedback == wgm_feedbacks::enum_hw_feedback::hw_success)
    {
        keyenceReady = true;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
    }
    else
    {
        keyenceReady = false;
        return wgm_feedbacks::enum_sub_sys_feedback::sub_error;
    }
}
/**
 * @brief
 *
 * @return double
 */
double  keyence_sensor::keyence_client_get_value_output0()
{
    std::cout << "get reading keyence sensor output0" << std::endl;
    if (keyence_last_mesured_output0.size() > 10) keyence_last_mesured_output0.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(0);
    if (current_value == 0) return 0;
    keyence_last_mesured_output0.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output0.front() << std::endl;
    return current_value;

}

/**
 * @brief
 *
 * @return double
 */
double  keyence_sensor::keyence_client_get_value_output1()
{

    std::cout << "get reading keyence sensor output1" << std::endl;
    if (keyence_last_mesured_output1.size() > 10) keyence_last_mesured_output1.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(1);
    if (current_value == 0) return 0;
    keyence_last_mesured_output1.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output1.front() << std::endl;
    return current_value;

}
/**
 * @brief
 *
 * @return double
 */
double  keyence_sensor::keyence_client_get_value_output2()
{

    std::cout << "get reading keyence sensor output2" << std::endl;
    if (keyence_last_mesured_output2.size() > 10) keyence_last_mesured_output2.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(2);
    if (current_value == 0) return 0;
    keyence_last_mesured_output2.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output2.front() << std::endl;
    return current_value;

}
/**
 * @brief
 *
 */
void keyence_sensor::keyence_client_get_value_all()
{
    keyence_client_get_value_output0();
    keyence_client_get_value_output1();
    keyence_client_get_value_output2();
}
