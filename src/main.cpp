// A C++ program that compiles and runs another C++
// program
#include "Windows.h"
#include "iostream"

int main ()
{
	LPCWSTR pyFilePath = L"C:\\Users\\SamiDhiab\\Theion_Repos\\soft_wafer_holder_system_controller\\dependencies\\software_repetier_rest_api\\src\\repetier_manager_lib\\repetier_manager.py";
	LPCWSTR pyCmd = L"C:\\Users\\SamiDhiab\\AppData\\Local\\Programs\\Python\\Python39\\python.exe";
	LPCWSTR cppFile =L"C:\\Users\\SamiDhiab\\Theion_Repos\\soft_wafer_holder_system_controller\\dependencies\\lib_keyence_distance_sensor\\build\\keyence_bin.exe";
	std::cout << "Running keyence program "<<std::endl;
	auto retVALcpp= ShellExecuteW(NULL, L"open", cppFile, NULL, NULL, SW_SHOWDEFAULT);
	std::cout << "Running delta program "<<std::endl;
	auto retVALpy = ShellExecuteW(NULL, L"open", pyCmd, pyFilePath, NULL, SW_SHOWDEFAULT);
	return 0;
}
