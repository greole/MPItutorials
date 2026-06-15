#include <mpi.h>
#include <cstdio>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0)
            fprintf(stderr, "Error: need at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    const int tag = 0;
    if (rank == 0) {
        int value = 42;
        MPI_Send(&value, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        printf("Rank 0 sent: %d\n", value);
    } else if (rank == 1) {
        int value;
        MPI_Recv(&value, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received: %d\n", value);
    }

    MPI_Finalize();
    return 0;
}
