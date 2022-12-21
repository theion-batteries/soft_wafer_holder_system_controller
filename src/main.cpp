// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main(int argc, char* argv[])
{
	whs_controller wafer_sys_control;
	wafer_sys_control.get_axis_ptr()->connect();
	// axis
	while (1)
	{
		std::string cmd;
		std::cout << "Enter command: "; // no flush needed
		std::cin >> cmd;
		auto reply = wafer_sys_control.sendDirectCmdAxis(cmd);
		std::cout << reply;
	}


	//// keyence
	//wafer_sys_control.keyence_client_connect();
	//wafer_sys_control.keyence_client_get_value_all(); //ready

	// controller  algorithm
	//wafer_sys_control.move_down_until_data_availble();
	//wafer_sys_control.move_down_to_surface();
	//wafer_sys_control.deep_wafer_holder_desired_thickness();
	return 0;
}
