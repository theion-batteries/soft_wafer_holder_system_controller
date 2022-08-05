# soft_wafer_holder_system_controller
contains wafer holder system controller source code

## Concept

the image below explains how concept is made:

![concept](docs/whs_controller.jpg)

the wafer holder motion system is based on two subsystem:
- delta motion system
- keyence distance sensor controller

these 2 subsystems shall run on independant processes and communicate with a controller process via socket.

the image below show some output after running first tests:
![test](docs/whs_controller.png)

## Design


## Tests







## Q&A
if cmake failed to build due to socket dependecies problems, do the following
~~~cpp
// in file dependencies\lib_keyence_distance_sensor\
dependencies\sockpp\include\sockpp\platform.h
// paste these lines anywhere

/* Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib*/
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
~~~