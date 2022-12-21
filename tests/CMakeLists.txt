include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(hello_test
  main.cpp
  test.h
)
target_link_libraries(hello_test
  GTest::gtest_main
  whs_static
)
target_include_directories(hello_test
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
)
include(GoogleTest)
gtest_discover_tests(hello_test)
