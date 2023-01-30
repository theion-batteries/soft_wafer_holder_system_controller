// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"

int main(int argc, char* argv[])
{
	whs_controller wafer_sys_control;
	wafer_sys_control.reset_config_file();
	return 0;
}
