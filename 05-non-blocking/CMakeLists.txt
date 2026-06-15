add_executable(isend_irecv isend_irecv.cpp)
target_link_libraries(isend_irecv MPI::MPI_CXX)

add_executable(ialltoallv ialltoallv.cpp)
target_link_libraries(ialltoallv MPI::MPI_CXX)
