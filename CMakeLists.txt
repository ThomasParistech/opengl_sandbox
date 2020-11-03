cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
project(OpenGL-Sandbox)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Output directory of all executables.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(inc)
add_subdirectory(src)
add_subdirectory(app)
