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
	//wafer_sys_control.move_down_until_data_availble(config["mm_steps"].as<double>(), config["delay_to_move_request"].as<DWORD>());
	//wafer_sys_control.move_down_to_surface(config["ref_dis"].as<double>());
	//wafer_sys_control.deep_wafer_holder_desired_thickness(config["thickness"].as<double>(), config["mm_step_res"].as<double>());
	return 0;
}
