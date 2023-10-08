cmake_minimum_required(VERSION 3.18)
project(cdcl_sat_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
include_directories(include)
add_executable(cdcl_sat_solver tests/main.cpp src/Entities.cpp src/CDCL.cpp src/Dimacs_parser.cpp)
add_executable(solveSAT src/Entities.cpp src/CDCL.cpp src/SolveSAT.cpp src/Dimacs_parser.cpp)
add_executable(solveCircuitSAT src/Entities.cpp src/CDCL.cpp src/SolveCircuitSAT.cpp src/Bench_parser.cpp src/Tseitin_transformation.cpp)