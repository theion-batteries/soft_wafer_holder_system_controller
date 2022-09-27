// A C++ program that compiles and runs another C++ and Python apps
// program

#include "whs_controller.h"
#include "cmdParser.h"
#include "yaml-cpp/yaml.h"

int main(int argc, char* argv[])
{
	//cmdParser cli(argc, argv); // pass cmd line args to bject
	// thread test
	// laod config file
	YAML::Node config = YAML::LoadFile("./config.yaml");
	std::cout << "config file loaded, printing parameters: " << std::endl;
	std::cout << "mm_steps: " << config["mm_steps"].as<std::string>() << std::endl;
	std::cout << "delay_to_move_request: " << config["delay_to_move_request"].as<std::string>() << std::endl;
	std::cout << "ref_dis: " << config["ref_dis"].as<std::string>() << std::endl;
	std::cout << "thickness: " << config["thickness"].as<std::string>() << std::endl;
	std::cout << "mm_step_res: " << config["mm_step_res"].as<std::string>() << std::endl;
	std::cout << "python path: " << config["python_path"].as<std::string>() << std::endl;
	std::cout << "python script: " << config["python_script"].as<std::string>() << std::endl;
	auto path =  (config["python_path"].as<std::string>() );
	auto script = (config["python_script"].as<std::string>());
	std::wstring tempPath = std::wstring(path.begin(), path.end());
    std::wstring tempScript = std::wstring( script.begin(),  script.end());
	whs_controller wafer_sys_control( tempPath.c_str(), tempScript.c_str());

	// delta
	wafer_sys_control.run_delta_subprocess();
	wafer_sys_control.connect_to_delta_server(); // ready
	wafer_sys_control.move_delta_home(); 
	// keyence
	wafer_sys_control.keyence_client_connect();
	wafer_sys_control.keyence_client_get_value_all(); //ready
	// controller  algorithm
	wafer_sys_control.move_down_until_data_availble(config["mm_steps"].as<double>(), config["delay_to_move_request"].as<DWORD>());
	wafer_sys_control.move_down_to_surface(config["ref_dis"].as<double>());
	wafer_sys_control.deep_wafer_holder_desired_thickness(config["thickness"].as<double>(), config["mm_step_res"].as<double>());
	return 0;
}
