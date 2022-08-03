// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main ()
{
	whs_controller wafer_sys_control;
	//wafer_sys_control.run_keyence_subprocess(); // to run subprocesses
	//wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_keyence_server(); // to connect to test server
	wafer_sys_control.set_keyence_mesurement_mode();
	wafer_sys_control.get_keyence_sensor_mesured_Values();

	wafer_sys_control.connect_to_delta_server();
	wafer_sys_control.get_delta_position();
	return 0;
}
