# soft_wafer_holder_system_controller
contains wafer holder system controller source code

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