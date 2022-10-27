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
add_subdirectory(dependencies/lib_keyence_sdk )
#add_subdirectory(dependencies/sockpp EXCLUDE_FROM_ALL)
#add_subdirectory(dependencies/yaml-cpp EXCLUDE_FROM_ALL)

set(CMAKE_PREFIX_PATH "C:/Users/SamiDhiab/vcpkg/installed/x64-windows/share")

find_package(yaml-cpp CONFIG REQUIRED)
find_package(sockpp CONFIG REQUIRED)

###################### Build Library ############################

# source *.cpp
file(GLOB lib_SRCS
        "${PROJECT_SOURCE_DIR}/src/whs_controller.cpp"
)
# add libraries
add_library(whs_static STATIC ${lib_SRCS})
# link againt static lib
target_link_libraries(whs_static 
# keyence interface lib
PUBLIC keyence_lib_static
# sockpp lib
PRIVATE ${SOCKPP_LIBRARIES}
#yaml lib
PRIVATE yaml-cpp

)
# include libs
target_include_directories(whs_static 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib_keyence_sdk/includes
PRIVATE ${SOCKPP_INCLUDE_DIRS}
)
############################# build executable  ###################################

## source *.cpp
#file(GLOB exe_SRCS
#        "${PROJECT_SOURCE_DIR}/src/*.cpp"
#)
## add .exe
#add_executable(whs_exe ${exe_SRCS})
#
## link againt static lib
#target_link_libraries(whs_exe 
## keyence interface lib
#${PROJECT_SOURCE_DIR}/build/dependencies/lib_keyence_sdk/Debug/keyence_lib_static.lib
## keyence sdk lib
#${PROJECT_SOURCE_DIR}/dependencies/lib_keyence_sdk/Samples/VC_Src/Libx64/LKIF2.lib
## sockpp lib
#${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
## yaml lib
#${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/libyaml-cppmdd.lib
#
#)
#


