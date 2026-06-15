#include <mpi.h>
#include <cstdio>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // TODO implement MPI_Comm_size

    // TODO add %d size to prinft
    printf("Hello world from rank %d\n", rank);

    MPI_Finalize();
    return 0;
}
