// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main(int argc, char* argv[])
{
	whs_controller wafer_sys_control;
	// delta
	wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_delta_server(); // ready
	wafer_sys_control.move_delta_home(); 
	// keyence
	wafer_sys_control.keyence_client_connect();
	wafer_sys_control.keyence_client_get_value_all(); //ready
	// controller  algorithm
	wafer_sys_control.move_down_until_data_availble();
	wafer_sys_control.move_down_to_surface();
	wafer_sys_control.deep_wafer_holder_desired_thickness();
	return 0;
}
