// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"
int main ()
{
	whs_controller wafer_sys_control;
	wafer_sys_control.run_keyence_subprocess();
	wafer_sys_control.run_delta_subprocess();

	return 0;
}
