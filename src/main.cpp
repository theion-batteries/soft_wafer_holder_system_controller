// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main()
{
	whs_controller wafer_sys_control;
	// delta
	wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_delta_server();
	wafer_sys_control.get_delta_position();
	wafer_sys_control.move_delta_home(); // ready
	wafer_sys_control.get_delta_position(); // test pos

	//// keyence
	wafer_sys_control.keyence_client_connect();
	wafer_sys_control.keyence_client_get_value_all(); //ready
	//// controller 
	wafer_sys_control.move_down_until_data_availble();
	




	return 0;
}
