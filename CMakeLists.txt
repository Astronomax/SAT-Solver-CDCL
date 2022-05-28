cmake_minimum_required(VERSION 3.22)
project(cdcl_sat_solver)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
add_executable(cdcl_sat_solver tests/main.cpp src/Entities.cpp src/CDCL.cpp tests/dimacs_parser.cpp)
add_executable(hand_test  src/Entities.cpp src/CDCL.cpp src/submain.cpp)