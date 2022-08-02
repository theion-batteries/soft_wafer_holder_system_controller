// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"
void debug();
int main ()
{


	whs_controller wafer_sys_control;
	wafer_sys_control.run_keyence_subprocess();
	wafer_sys_control.run_delta_subprocess();

	return 0;
}

void debug()
{
	LPCWSTR pyFilePath = L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/software_repetier_rest_api/src/repetier_manager_lib/command_parser.py";
	LPCWSTR pyCmd = L"C:/Users/SamiDhiab/AppData/Local/Programs/Python/Python39/python.exe";
	LPCWSTR cppFile =L"C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/lib_keyence_distance_sensor/build/Debug/keyence_bin.exe";
   	   std::cout << "Running delta program " <<std::endl;

	ShellExecuteW(NULL, L"open", pyCmd, pyFilePath, NULL, SW_SHOWDEFAULT);
       std::cout << "Running keyence program " <<std::endl;

	ShellExecuteW(NULL, L"open", cppFile, NULL, NULL, SW_SHOWDEFAULT); 
}