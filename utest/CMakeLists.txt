# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in sim
add_executable(utest_exec
        block_test.cpp
        grid_test.cpp
        progargs_test.cpp
        particle_test.cpp)
# Library dependencies
target_link_libraries (utest_exec
        PRIVATE
        sim
        GTest::gtest_main
        Microsoft.GSL::GSL)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(utest_exec)
