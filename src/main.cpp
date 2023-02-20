#include "whs_controller.h"
#include <iostream>
#include <string>

enum options {
	CLOSE = 0,
	OPEN_CONFIG = 1,
	RESET_CONFIG = 2,
	CONNECT = 3,
	SEND_CMD = 4,
	CHANGE_IP = 5,
	RUN_ALGO1 = 6,
	RUN_ALGO2 = 7,
	RUN_ALGO3 = 8,
};

int main(int argc, char* argv[]) {
	whs_controller wafer_sys_control = whs_controller("127.0.0.1", 8882, "127.0.0.1", 24687);
	options choices = CLOSE;
	int choice = -1;
	std::string cmd = "";
	std::string ip = "";

	while (choice != CLOSE) {
		std::cout << "Please choose an option: \n";
		std::cout << "0: CLOSE\n";
		std::cout << "1: OPEN_CONFIG\n";
		std::cout << "2: RESET_CONFIG\n";
		std::cout << "3: CONNECT\n";
		std::cout << "4: SEND_CMD\n";
		std::cout << "5: CHANGE_IP\n";
		std::cout << "6: ALGO1: move down until data valid\n";
		std::cout << "7: ALGO2: move down to surface\n";
		std::cout << "8: ALGO3: deep wafer thickness\n";
		std::cin >> choice;

		switch (choice) {
		case CLOSE:
			break;
		case OPEN_CONFIG:
			wafer_sys_control.open_config_file();
			break;
		case RESET_CONFIG:
			wafer_sys_control.reset_config_file();
			break;
		case CONNECT:
			wafer_sys_control.connect_controller();
			break;
		case SEND_CMD:
			std::cout << "Enter command: ";
			std::cin >> cmd;
			std::cout << wafer_sys_control.sendDirectCmdAxis(cmd) << std::endl;
			break;
		case CHANGE_IP:
			std::cout << "Enter IP: ";
			std::cin >> ip;
			std::cout << "New IP: " << ip << std::endl;
			break;
		case RUN_ALGO1:
			wafer_sys_control.move_down_until_data_availble();
			break;
		case RUN_ALGO2:
			wafer_sys_control.move_down_to_surface();
			break;
		case RUN_ALGO3:
			wafer_sys_control.deep_wafer_holder_desired_thickness();
			break;
		default:
			std::cout << "Invalid option. Please choose again.\n";
			break;
		}
	}

	return 0;
}
