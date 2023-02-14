include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(whsTests
  main.cpp
)
target_link_libraries(whsTests
  GTest::gtest_main
  GTest::gmock_main
  whsLib
)
target_include_directories(whsTests
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
)
#add_test(NAME whsTests COMMAND whsTests)
#set_tests_properties(whsTests PROPERTIES OUTPUT_ON_SUCCESS TRUE)

enable_testing()
include(GoogleTest)
gtest_discover_tests(whsTests)