# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.1.0)

# project name
project(whs_controller VERSION "1.0")

# set the C++17 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()
#This is necessary for MSVC to create a symbol file, .lib, besides a shared library, .dll
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
########################## add subdirectory##################
set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-windows/share")
message(" child: vcpkg root folder: ${VCPKG_ROOT}") 
find_package(sockpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
# add dependencies cmake
add_subdirectory(dependencies/lib_keyence_sdk )
add_subdirectory(tests)
add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)  
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)  
################## search and append ##########################
find_file(whs_config 
        NAMES whs_config.yaml
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/config
        REQUIRED
)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development )
message("found whs config file: ${whs_config}")        
set (WHS_CONFIG  "${whs_config}" CACHE FILEPATH "${whs_config}")
set(WHS_CONFIG_DIR ${CMAKE_INSTALL_PREFIX}/config/whs_config.yaml CACHE FILEPATH "${CMAKE_INSTALL_PREFIX}/config/whs_config.yaml") 
add_definitions(-DWHS_CONFIG="${WHS_CONFIG_DIR}") 
# copy config one by one 
function(copy_whs_config target_exe config_file)
add_custom_command(TARGET ${target_exe} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy ${config_file} ${WHS_CONFIG_DIR}  
COMMENT "copy config from ${config_file} to ${WHS_CONFIG_DIR}"
    )
endfunction()
# copy dlls one by one 
function(copy_dlls_yaml_target_destination target_exe)
add_custom_command(TARGET ${target_exe} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different  $<TARGET_FILE:yaml-cpp> $<TARGET_FILE_DIR:${target_exe}>
COMMAND_EXPAND_LISTS)
endfunction()
###################### Build Library ############################
# source *.cpp
file(GLOB lib_SRCS
        "${PROJECT_SOURCE_DIR}/src/whs_controller.cpp"
        "${PROJECT_SOURCE_DIR}/src/delta_motion.cpp"
        "${PROJECT_SOURCE_DIR}/src/Iaxis_motion.cpp"
        "${PROJECT_SOURCE_DIR}/src/Idistance_sensor.cpp"
        "${PROJECT_SOURCE_DIR}/src/linear_motion.cpp"
        "${PROJECT_SOURCE_DIR}/src/keyence_sensor.cpp"
        # mocking files
        #"${PROJECT_SOURCE_DIR}/tests/sensorMock.cpp"
        #"${PROJECT_SOURCE_DIR}/tests/axisMock.cpp"
)
# add libraries
add_library(whs_static STATIC ${lib_SRCS})
# link againt static lib
target_link_libraries(whs_static 
# keyence interface lib
PUBLIC keyence_lib_static
PUBLIC wsock32 
PUBLIC ws2_32
# sockpp lib
PUBLIC ${SOCKPP_LIBRARIES}
#yaml lib
PUBLIC yaml-cpp
# py
PRIVATE ${Python_LIBRARIES}
)
# include libs
target_include_directories(whs_static 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib_keyence_sdk/includes
PUBLIC ${SOCKPP_INCLUDE_DIRS}
PRIVATE ${Python_INCLUDE_DIRS}
)
target_link_directories(whs_static PRIVATE
        ${Python_LIBRARY_DIRS})
copy_whs_config(whs_static ${WHS_CONFIG})

############################# build executable ###################################
## source *.cpp
file(GLOB exe_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        # mocking files
        "${PROJECT_SOURCE_DIR}/tests/sensorMock.cpp"
        "${PROJECT_SOURCE_DIR}/tests/axisMock.cpp"
)
# add .exe
add_executable(whs_exe ${exe_SRCS})
target_link_libraries(whs_exe 
# keyence interface lib
PUBLIC keyence_lib_static
PUBLIC wsock32 
PUBLIC ws2_32
# sockpp lib
PUBLIC ${SOCKPP_LIBRARIES}
#yaml lib
PUBLIC yaml-cpp
# py
PRIVATE ${Python_LIBRARIES}
)
# include libs
target_include_directories(whs_exe 
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib_keyence_sdk/includes
PUBLIC ${SOCKPP_INCLUDE_DIRS}
PRIVATE ${Python_INCLUDE_DIRS}
)

copy_dlls_keyence_target_destination(whs_exe)
copy_whs_config(whs_exe ${WHS_CONFIG})
copy_dlls_yaml_target_destination(whs_exe)

############################# install targets ########################################
        FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h")
        INSTALL(FILES ${files} DESTINATION include/whs_static/)
        install(TARGETS whs_static whs_exe 
        CONFIGURATIONS Debug
        RUNTIME DESTINATION bin/Debug
        LIBRARY DESTINATION lib)

        FILE(GLOB yamlDll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/yaml-cppd.dll" )
        install(FILES ${yamlDll} DESTINATION bin/Debug)

        
        FILE(GLOB yamlDll "${CMAKE_CURRENT_BINARY_DIR}/Debug/yaml-cppd.dll" )
        install(FILES ${yamlDll} DESTINATION bin/Debug)