cmake_minimum_required(VERSION 3.10)

set(TARGET glad)
project(${TARGET} LANGUAGES C)

set(TARGET_ARCHITECTURE "x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/bin)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(GLAD_SRC_FILES 
  src/glad.c
)

add_compile_options(
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

add_library(${TARGET} ${GLAD_SRC_FILES})

target_include_directories(${TARGET} PRIVATE
  include
)

target_link_libraries(${TARGET} PRIVATE 
  GL 
)
