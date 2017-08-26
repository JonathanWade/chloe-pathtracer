cmake_minimum_required (VERSION 3.5)
project (Pathtracer CXX)

include_directories("$(PROJECT_SOURCE_DIR)/includes")

set (CMAKE_CXX_STANDARD 11)
add_executable(pathtracer main.cc)

