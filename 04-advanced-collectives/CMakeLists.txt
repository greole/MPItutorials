add_executable(alltoallv alltoallv.cpp)
target_link_libraries(alltoallv MPI::MPI_CXX)

add_executable(alltoallv_gather alltoallv_gather.cpp)
target_link_libraries(alltoallv_gather MPI::MPI_CXX)
