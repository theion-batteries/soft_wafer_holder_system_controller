include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(whsTests
  main.cpp
)
target_link_libraries(whsTests
  GTest::gtest_main
  whs_static
)
target_include_directories(whsTests
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
)
include(GoogleTest)
gtest_discover_tests(whsTests)

#add_test(NAME whsTests COMMAND whsTests)
enable_testing()
