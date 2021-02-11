cmake_minimum_required (VERSION 3.15)
project (CanopenStack VERSION 4.1.5)

# Make CANopen library
add_subdirectory(canopen)
add_subdirectory(driver)

# Make applications
add_subdirectory(testsuite)
add_subdirectory(example)
