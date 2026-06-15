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

    const int rounds = 4;
    int value = 0;

    if (rank == 0) {
        for (int i = 0; i < rounds; i++) {
            MPI_Send(&value, 1, MPI_INT, 1, i, MPI_COMM_WORLD);
            printf("Rank 0 sent:     %d  (round %d)\n", value, i);
            MPI_Recv(&value, 1, MPI_INT, 1, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank 0 received: %d  (round %d)\n", value, i);
        }
    } else if (rank == 1) {
        for (int i = 0; i < rounds; i++) {
            MPI_Recv(&value, 1, MPI_INT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank 1 received: %d  (round %d)\n", value, i);
            value++;
            MPI_Send(&value, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
            printf("Rank 1 sent:     %d  (round %d)\n", value, i);
        }
    }

    MPI_Finalize();
    return 0;
}
