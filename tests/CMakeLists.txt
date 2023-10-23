cmake_minimum_required(VERSION 3.10)

set(NAMESPACE cbased)
project(${NAMESPACE}-tests LANGUAGES C)

set(TARGET_ARCHITECTURE "x64")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -Wextra -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-macro-redefined -Wno-attributes -Wno-incompatible-library-redeclaration")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(
  .
  ${CMAKE_SOURCE_DIR}/cbased/src
  ${CMAKE_SOURCE_DIR}/vendor/Unity/src
)

if (DEFINED ENV{PG_INCLUDE_DIR})
    include_directories($ENV{PG_INCLUDE_DIR})
else()
    include_directories(/usr/include/postgresql)
endif()

link_libraries(cbased unity)

file(GLOB_RECURSE TEST_SRC_FILES test_*.c)

add_custom_target(${NAMESPACE}-run_tests)

foreach(test_file ${TEST_SRC_FILES})
  get_filename_component(test_name ${test_file} NAME_WE)
  set(generated_file ${NAMESPACE}-${test_name}_gen.c)

  add_custom_command(
    OUTPUT ${generated_file}
    COMMAND ${CMAKE_SOURCE_DIR}/vendor/Unity/auto/generate_test_runner.rb ${test_file} ${generated_file}
    DEPENDS ${test_file}
  )

  add_executable(${NAMESPACE}-${test_name}.bin ${test_file} ${generated_file})
  add_custom_target(${NAMESPACE}-${test_name} DEPENDS ${generated_file})
  add_dependencies(${NAMESPACE}-${test_name}.bin ${NAMESPACE}-${test_name})

  add_custom_command(
    COMMENT "Running ${NAMESPACE}-${test_name}.bin"
    TARGET ${NAMESPACE}-run_tests POST_BUILD
    DEPENDS ${NAMESPACE}-${test_name}.bin
    COMMAND $<TARGET_FILE:${NAMESPACE}-${test_name}.bin>
  )

  add_custom_target(${NAMESPACE}-run_${test_name}
    COMMENT "Running ${NAMESPACE}-${test_name}.bin"
    DEPENDS ${NAMESPACE}-${test_name}.bin
    COMMAND $<TARGET_FILE:${NAMESPACE}-${test_name}.bin>
  )

  # add_custom_command(
  #   COMMENT "Running ${NAMESPACE}-${test_name}.bin"
  #   TARGET ${NAMESPACE}-${test_name}.bin POST_BUILD
  #   COMMAND $<TARGET_FILE:${NAMESPACE}-${test_name}.bin>
  # )

  # add_custom_target(${NAMESPACE}-run_${test_name} ALL
  #   COMMENT "Running ${NAMESPACE}-${test_name}.bin"
  #   DEPENDS ${NAMESPACE}-${test_name}.bin
  #   COMMAND $<TARGET_FILE:${NAMESPACE}-${test_name}.bin>
  # )
endforeach()

add_dependencies(run_tests ${NAMESPACE}-run_tests)
