# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/dependencies/yaml-cpp/test/gtest-1.8.0"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/src/googletest_project-build"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/prefix"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/tmp"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/src/googletest_project-stamp"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/src"
  "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/src/googletest_project-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/SamiDhiab/Theion_Repos/soft_wafer_holder_system_controller/build/dependencies/yaml-cpp/test/googletest_project-prefix/src/googletest_project-stamp/${subDir}")
endforeach()
