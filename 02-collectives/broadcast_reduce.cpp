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

    // Broadcast: root sends its rank to everyone
    int value = (rank == root) ? rank : -1;
    MPI_Bcast(&value, 1, MPI_INT, root, MPI_COMM_WORLD);
    printf("Rank %d received: %d\n", rank, value);

    // Reduce: sum all values to rank 0
    int sum = 0;
    MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Rank 0 reduce sum: %d\n", sum);

    MPI_Finalize();
    return 0;
}
