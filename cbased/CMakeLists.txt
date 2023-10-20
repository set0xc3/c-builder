cmake_minimum_required(VERSION 3.10)

set(TARGET cbased)
project(${TARGET} LANGUAGES C)

set(TARGET_ARCHITECTURE "x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET}/bin)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SRC_FILES       src/*.c)

find_package(SDL2 REQUIRED)

if (DEFINED ENV{PG_INCLUDE_DIR})
    include_directories($ENV{PG_INCLUDE_DIR})
else()
    include_directories(/usr/include/postgresql)
endif()


add_library(${TARGET} ${SRC_FILES})

target_compile_options(${TARGET} PRIVATE 
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

target_include_directories(${TARGET} PRIVATE 
  src 
  ${CMAKE_SOURCE_DIR}/vendor/glad/include
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${TARGET} PRIVATE 
  vulkan
  pq
  glad
  ${SDL2_LIBRARIES}
)

# add_custom_target(build-shaders
#   COMMAND mkdir -p ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
#   COMMAND /usr/bin/glslc assets/shaders/default.vert -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/default.vert.spv
#   COMMAND /usr/bin/glslc assets/shaders/default.frag -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/default.frag.spv
# )
