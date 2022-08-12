// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main()
{
	whs_controller wafer_sys_control;
	// delta
	wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_delta_server();
	wafer_sys_control.move_delta_home(); // ready
	// keyence
	wafer_sys_control.keyence_client_connect();
	wafer_sys_control.keyence_client_get_value_all(); //ready
	// controller  algorithm
	wafer_sys_control.move_down_until_data_availble();
	wafer_sys_control.move_down_to_surface(158);
	wafer_sys_control.deep_wafer_holder_desired_thickness(0.1, 0.01);
	return 0;
}
