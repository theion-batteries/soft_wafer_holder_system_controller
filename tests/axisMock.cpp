#include "axisMock.h"


axisMock::axisMock(/* args */)
{
    std::cout << "creating linear axis  mock" << std::endl;
}

axisMock::~axisMock()
{

}
std::string axisMock::sendDirectCmd(std::string& cmd)
{
    std::cout << "sending command " << cmd << std::endl;
    for (auto& command : axis_cmds)
    {
        if (cmd == command.second)
        {
            std::cout << "command found" << std::endl;
            return "ok";
        }
    }
    std::cout << "command not found" << std::endl;
    return "NA";
}

bool axisMock::getStatus()
{
    return true;
}


/**
 * @brief TODO: change the function call
 *
 * @param new_position
 */
void axisMock::move_to(int new_position)
{

    move_up_by(new_position);

}

wgm_feedbacks::enum_sub_sys_feedback axisMock::connect()
{
    std::cout << "connecting axis mock" << std::endl;
    return wgm_feedbacks::enum_sub_sys_feedback::sub_success;

}

void axisMock::disconnect()
{
    std::cout << "disconnecting axis mock" << std::endl;
}


/**
 * @brief
 *
 * @return double
 */
double axisMock::get_position()
{
    std::cout << "get axis mock curent position" << std::endl;

    return currentPos;
}
/**
 * @brief
 *
 */
void axisMock::move_home()
{
    std::cout << "moving home" << std::endl;
    currentPos = 300;
}
/**
 * @brief
 *
 */
void axisMock::get_speed()
{

}
void axisMock::set_speed(double_t new_val)
{

}
void axisMock::move_up_to(double_t new_pos)
{

}
void axisMock::move_down_to(double_t new_pos)
{

}
/**
 * @brief
 *
 * @param steps
 */
void axisMock::move_up_by(double_t steps)
{
    std::cout << "moving up by " << steps << std::endl;
    currentPos=currentPos+steps ;
    std::cout << "new pos "<< currentPos << std::endl;

}

/**
 * @brief
 *
 * @param steps
 */
void axisMock::move_down_by(double_t steps)
{

    std::cout << "moving down by " << steps << std::endl;
   currentPos= currentPos-steps;
    std::cout << "new pos "<< currentPos << std::endl;

}

void axisMock::move_center()
{
    std::cout << "moving center" << std::endl;
}

void axisMock::unlock()
{
    std::cout << "unlock device" << std::endl;

}