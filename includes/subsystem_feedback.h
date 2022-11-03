/**
 * @file feedback_management.h
 * @author sami dhiab (sami@theion.de)
 * @brief feedback managment file
 * @version 0.1
 * @date 2022-07-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <iostream>
#include "hardware_feedback.h"

namespace wgm_feedbacks
{
    /********* enumeration classes ***********/
  
    enum class enum_sub_sys_feedback
    {
        sub_success = 3,
        sub_error = -2
    };
 
    /*********** feedback classes *************/
    
     class sub_sys_feedback
    {
        private:
            enum_sub_sys_feedback sub_sys_feed_val;
            hw_feedback* hw_feed;
    };
 

}


