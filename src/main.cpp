// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main()
{
	whs_controller wafer_sys_control;
	// keyence
	wafer_sys_control.keyence_client_connect();
	wafer_sys_control.keyence_client_get_value_output0();
	wafer_sys_control.keyence_client_get_value_output1();
	wafer_sys_control.keyence_client_get_value_output2();
	// delta
	wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_delta_server();
	wafer_sys_control.get_delta_position();



	return 0;
}
