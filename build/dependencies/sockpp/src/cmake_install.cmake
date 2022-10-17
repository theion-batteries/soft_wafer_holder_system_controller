<<<<<<< HEAD
<<<<<<<< HEAD:build/dependencies/sockpp/src/cmake_install.cmake
# Install script for directory: C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/sockpp/src
========
# Install script for directory: C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/yaml-cpp/test
>>>>>>>> f0496ee0a8cd6eb065b714087bc102259d9a4362:build/dependencies/yaml-cpp/test/cmake_install.cmake
=======
# Install script for directory: C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/sockpp/src
>>>>>>> f0496ee0a8cd6eb065b714087bc102259d9a4362

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/whs_controller")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

