add_executable(broadcast broadcast.cpp)
target_link_libraries(broadcast MPI::MPI_CXX)

add_executable(broadcast_reduce broadcast_reduce.cpp)
target_link_libraries(broadcast_reduce MPI::MPI_CXX)
