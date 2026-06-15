add_executable(send_recv send_recv.cpp)
target_link_libraries(send_recv MPI::MPI_CXX)

add_executable(ping_pong ping_pong.cpp)
target_link_libraries(ping_pong MPI::MPI_CXX)

add_executable(ring ring.cpp)
target_link_libraries(ring MPI::MPI_CXX)
