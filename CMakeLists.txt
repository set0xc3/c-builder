cmake_minimum_required(VERSION 3.10)

project(cb LANGUAGES C)

set(TARGET_ARCHITECTURE "x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SRC_FILES       ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE GLAD_SRC_FILES  ${CMAKE_SOURCE_DIR}/vendor/glad/src/*.c)

add_library(cb ${SRC_FILES} ${GLAD_SRC_FILES})

target_compile_options(cb PRIVATE 
  -g 
  -O0
  -Wall 
  -Wextra 
  -Wno-gnu-zero-variadic-macro-arguments 
  -Wno-unused-parameter 
  -Wno-unused-variable 
  -Wno-unused-but-set-variable 
  -Wno-macro-redefined 
  -Wno-attributes 
  -Wno-incompatible-library-redeclaration
)

target_include_directories(cb PRIVATE 
  ${CMAKE_SOURCE_DIR}/src 
  ${CMAKE_SOURCE_DIR}/vendor/glad/include
)

target_link_libraries(cb PRIVATE 
  m 
  GL 
  vulkan 
  SDL2
)

add_custom_target(build-shaders
  COMMAND mkdir -p ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
  COMMAND /usr/bin/glslc ${CMAKE_SOURCE_DIR}/assets/shaders/simple.vert -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/simple.vert.spv
  COMMAND /usr/bin/glslc ${CMAKE_SOURCE_DIR}/assets/shaders/simple.frag -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/simple.frag.spv
)

add_subdirectory(tests)
