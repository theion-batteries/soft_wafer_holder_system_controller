# soft_wafer_holder_system_controller
contains wafer holder system controller source code

* author: Sami dhiab
* email: sami@theion.de

## Concept (version 0.1)

the image below explains how concept is made:

![concept](docs/whs_controller.jpg)

the wafer holder motion system is based on two subsystem:
- delta motion system
- keyence distance sensor controller

these 2 subsystems shall run on independant processes and communicate with a controller process via socket.

the image below show some output after running first tests:
![test](docs/whs_controller.png)

## Concept (version 0.2)

After including the keyence sdk, the server bridge in the above foto is neglected and keyence lib sdk will be directly included in the whs controller class (without server) to controll the keyence controller and get mesurement. 

## dependencies

The whs controller lib depends on:
- 1. keyence_sdk submodule: see dependecies/
- 2. software_repetier_rest_api submodule: see dependencies/
1. keyence lib rely on 3 dlls (dynamic libaries). these dlls must be included in the same folder as the .exe file (see build/debug/) for debug or (build/release/) for release version. see dependencies/lib_keyence_sdk/includes
2. repetier (delta) lib rely on:
    - pip modules: see requirements.txt
    - delta_server.py: run this server as subprocess and process data. see dependencies/software_repetier_rest_api/src/repetier_manager_lib/
## Design

the design should rely on mvc pattern (model-view-control) to comply with the high level architecture and the user interface later.
Details can be found in the parent folder.

## Tests
unit tests will included under tests/ folder to test teh following:
- 
- 






## Q&A
1. if cmake failed to build due to socket dependecies problems, do the following
~~~cpp
// in file dependencies/lib_keyence_distance_sensor/dependencies/sockpp/include/sockpp/platform.h
// paste these lines anywhere

/* Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib*/
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
~~~
2. if the executable doesn't run properly, check if the dlls are put in the same folder as teh .exe file