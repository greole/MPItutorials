cmake_minimum_required(VERSION 3.10)
project(MPIExamples CXX)

find_package(MPI REQUIRED)

add_subdirectory(01-hello-world)
add_subdirectory(02-collectives)
add_subdirectory(03-point-to-point)
