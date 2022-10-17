# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.0)

# project name
project(whs_controller VERSION "1.0")


# set the C++17 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add dependencies cmake
add_subdirectory(dependencies/lib_keyence_distance_sensor/dependencies/sockpp/)
add_subdirectory(dependencies/lib_keyence_sdk)
add_subdirectory(dependencies/yaml-cpp)

## library name
set(WHS_CONTROLLER whs_controller)
set(WHS_CONTROLLER_STATIC ${whs_controller}-static)

# --- Build Options ---

if(WIN32)
	option(WHS_BUILD_STATIC "Build static library" ON)
	option(WHS_BUILD_SHARED "Build shared library (DLL)" OFF)
else()
	option(WHS_BUILD_STATIC "Build static library" OFF)
	option(WHS_BUILD_SHARED "Build shared library" ON)
endif()



# include libs
include_directories(${PROJECT_SOURCE_DIR}/includes/)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_distance_sensor/dependencies/sockpp/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/includes)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/include/)

# source *.cpp
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/src/*.cpp"
		"${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_distance_sensor/dependencies/sockpp/src/*.cpp"

)
# add .exe
add_executable(whs_controller ${all_SRCS})
# link againt static lib
target_link_libraries(whs_controller ${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/LKIF2.lib
									 ${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/build/Debug/libyaml-cppmdd.lib

)


