# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.0)

# project name
project(whs_controller VERSION "1.0")

# set the C++17 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#This is necessary for MSVC to create a symbol file, .lib, besides a shared library, .dll
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
# add dependencies cmake
#add_subdirectory(dependencies/sockpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk )#dependencies/lib_keyence_sdk/build)
#add_subdirectory(dependencies/yaml-cpp)


# include libs
include_directories(${PROJECT_SOURCE_DIR}/includes/)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/sockpp/include)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/includes)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/include/)

# source *.cpp
file(GLOB exe_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/src/*.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/src/*.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/sockpp/src/*.cpp"

)
# source *.cpp
file(GLOB lib_SRCS
        "${PROJECT_SOURCE_DIR}/src/whs_controller.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/src/*.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/src/*.cpp"
		#"${PROJECT_SOURCE_DIR}/dependencies/sockpp/src/*.cpp"

)

# add libraries
add_library(whs_static STATIC ${lib_SRCS})
# add .exe
add_executable(whs_exe ${exe_SRCS})

# link againt static lib
target_link_libraries(whs_static 
# keyence interface lib
${PROJECT_SOURCE_DIR}/build/dependencies/lib_keyence_sdk/Debug/keyence_lib_static.lib
# keyence sdk lib
${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/Samples/VC_Src/Libx64/LKIF2.lib
# sockpp lib
${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
# yaml lib
${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/libyaml-cppmdd.lib

)
# link againt static lib
target_link_libraries(whs_exe 
# keyence interface lib
${PROJECT_SOURCE_DIR}/build/dependencies/lib_keyence_sdk/Debug/keyence_lib_static.lib
# keyence sdk lib
${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/Samples/VC_Src/Libx64/LKIF2.lib
# sockpp lib
${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
# yaml lib
${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/libyaml-cppmdd.lib

)

#find_package(PythonInterp)
#message("999+
#    PYTHONINTERP_FOUND=${PYTHONINTERP_FOUND}
#    PYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
#    PYTHON_VERSION_STRING=${PYTHON_VERSION_STRING}
#")
#set(PyScript "${PROJECT_SOURCE_DIR}/dependencies/software_repetier_rest_api/src/repetier_manager_lib/delta_server.py")
#message("Executing python script: delta_server.py")
#
#execute_process(
#  COMMAND ${PYTHON_EXECUTABLE} ${PyScript}
##  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#)