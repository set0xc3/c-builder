cmake_minimum_required(VERSION 3.10)

project(tests LANGUAGES C)

set(TARGET_ARCHITECTURE "x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -Wextra -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-macro-redefined -Wno-attributes -Wno-incompatible-library-redeclaration")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(src)
link_libraries(m GL vulkan SDL2 cbased)

file(GLOB_RECURSE TESTS_SRC_FILES tests/*.c)

foreach(test_file ${TESTS_SRC_FILES})
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name}.bin ${test_file})
endforeach()

file(GLOB BINARIES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*.bin")
set(TEST_TARGETS "")

foreach(BINARY ${BINARIES})
  get_filename_component(BINARY_NAME ${BINARY} NAME_WE)
  add_custom_command(OUTPUT ${BINARY_NAME}.out
                     COMMAND ${BINARY}
                     DEPENDS ${BINARY}
                     COMMENT "Running ${BINARY}"
                     VERBATIM)
  add_custom_target(${BINARY_NAME} DEPENDS ${BINARY_NAME}.out)
  list(APPEND TEST_TARGETS ${BINARY_NAME})
endforeach()

add_custom_target(tests DEPENDS ${TEST_TARGETS})
