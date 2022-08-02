// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"
void run_test_server();
int main ()
{

	//run_test_server();
	whs_controller wafer_sys_control;
	//wafer_sys_control.run_keyence_subprocess();
	wafer_sys_control.connect_to_delta_server();
	//wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_keyence_server();

	return 0;
}

void run_test_server()
{
	LPCWSTR pyFilePath = L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/tests/server.py";
	LPCWSTR pyCmd = L"C:/Users/SamiDhiab/AppData/Local/Programs/Python/Python39/python.exe";
   	std::cout << "Running test server program " <<std::endl;
	ShellExecuteW(NULL, L"open", pyCmd, pyFilePath, NULL, SW_SHOWDEFAULT);
}