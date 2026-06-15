#include <mpi.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <root_rank>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int root = atoi(argv[1]);
    if (root < 0 || root >= size) {
        if (rank == 0)
            fprintf(stderr, "Error: root rank %d out of range [0, %d)\n", root, size);
        MPI_Finalize();
        return 1;
    }

    int value = (rank == root) ? rank : -1;

    // TODO implement
    // int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
    // here see https://rookiehpc.org/mpi/docs/mpi_bcast/index.html  for details
    // printf("Rank %d received: %d\n", rank, value);

    MPI_Finalize();
    return 0;
}
