cmake_minimum_required(VERSION 3.10)
project(cbased)

add_custom_target(run_tests)

add_subdirectory(vendor/Unity)
add_subdirectory(vendor/glad)
add_subdirectory(cbased)
add_subdirectory(tests)
