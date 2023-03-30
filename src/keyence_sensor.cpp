#include "keyence_sensor.h"


keyence_sensor::keyence_sensor(/* args */)
{
    std::cout << "creating keyence client" << "\n";

}
keyence_sensor::keyence_sensor(std::string ip, uint16_t port)
{

    std::cout << "creating keyence client" << "\n";

    _keyence_struct.ip = ip;
    _keyence_struct.port = port;
    Kclient = new keyence_client(_keyence_struct.ip);
    std::cout << "keyence server port:  " << _keyence_struct.port << "\n";

}

keyence_sensor::~keyence_sensor()
{
    delete Kclient;
}
bool keyence_sensor::getStatus()
{
    return keyenceReady;
}


wgm_feedbacks::enum_sub_sys_feedback keyence_sensor::disconnect()
{
    Kclient->disconnect();
    keyenceReady = false;

    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;
}
double keyence_sensor::getMesuredValue()
{
    //return keyence_client_get_value_output_all(); 
     return keyence_client_get_value_output0();    
    //return keyence_client_get_value_output1();
    // return keyence_client_get_value_output2(); 
    // return 0;   
}
/************************* keyence client methods*****************/
/**
 * @brief
 *
 */
wgm_feedbacks::enum_sub_sys_feedback keyence_sensor::connect()
{
    std::cout << "Running keyence client " << "\n";
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
    std::cout << "get reading keyence sensor output0 ";
    if (keyence_last_mesured_output0.size() > 10) keyence_last_mesured_output0.pop_front(); // remove last if 10
    double current_value = Kclient->get_value_output(0);
    std::cout<<"value :"<<current_value<<"\n";
    if (current_value == 0) return 0;
    keyence_last_mesured_output0.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output0.back() << "\n";
    return current_value;

}

/**
 * @brief
 *
 * @return double
 */
double  keyence_sensor::keyence_client_get_value_output1()
{

    std::cout << "get reading keyence sensor output1" << "\n";
    if (keyence_last_mesured_output1.size() > 10) keyence_last_mesured_output1.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(1);
    if (current_value == 0) return 0;
    keyence_last_mesured_output1.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output1.front() << "\n";
    return current_value;
}
/**
 * @brief
 *
 * @return double
 */
double  keyence_sensor::keyence_client_get_value_output2()
{

    std::cout << "get reading keyence sensor output2" << "\n";
    if (keyence_last_mesured_output2.size() > 10) keyence_last_mesured_output2.pop_back(); // remove last if 10
    double current_value = Kclient->get_value_output(2);
    if (current_value == 0) return 0;
    keyence_last_mesured_output2.push_back(current_value); // add to table
    std::cout << "value added to table " << keyence_last_mesured_output2.front() << "\n";
    return current_value;

}

double  keyence_sensor::keyence_client_get_value_output_all()
{

    std::cout << "get reading keyence sensor all ouputs" << "\n";
    double current_value = Kclient->get_value_all();
    if (current_value == 0) return 0;
    return current_value;

}
/**
 * @brief
 *
 */

