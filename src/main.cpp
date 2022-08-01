// A C++ program that compiles and runs another C++
// program
#include "Windows.h"
#include "iostream"

int main ()
{
	std::string pyFile ="repetier_manager.py";
	std::string pyFilePath = "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/software_repetier_rest_api/src/repetier_manager_lib/";
	std::string pyCmd = "python ";
	//cout << "Running keyence program ";
	//system("C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/lib_keyence_distance_sensor/build/keyence_bin.exe");
	std::cout << "Running delta program ";
	// shell execute 
	ShellExecute(hwnd, "find", "c:\\MyPrograms", NULL, NULL, 0);
	return 0;
}
