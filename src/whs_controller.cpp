#include "whs_controller.h"

whs_controller:: whs_controller(delta_server* delta, keyence_server* keyence){

    _delta_struct->ip=delta->ip;
    _delta_struct->port=delta->port;
    _keyence_struct->ip=keyence->ip;
    _keyence_struct->port=keyence->port;

}


void whs_controller::run_delta(){
	std::cout << "Running delta program "<<std::endl;
    std::string ip_arg = _delta_struct->ip;
    std::string port_arg = _delta_struct->port;
	auto retVALpy = ShellExecuteW(NULL, L"open", pyCmd, pyFilePath, NULL, SW_SHOWDEFAULT);
}
void whs_controller::run_keyence(){
	std::cout << "Running keyence program "<<std::endl;
    std::string ip_arg = _keyence_struct->ip;
    std::string port_arg = _keyence_struct->port;	
    auto retVALcpp= ShellExecuteW(NULL, L"open", cppFile, NULL, NULL, SW_SHOWDEFAULT);
}
void whs_controller::run_both()
{
    run_delta();
    run_keyence();
}

whs_controller::whs_controller(/* args */)
{
}

whs_controller::~whs_controller()
{
}
