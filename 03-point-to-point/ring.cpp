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

    int send_val = rank;
    int recv_val = -1;
    int dest = (rank + 1) % size;
    int src  = (rank - 1 + size) % size;

    MPI_Sendrecv(&send_val, 1, MPI_INT, dest, 0,
                 &recv_val, 1, MPI_INT, src,  0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Rank %d: sent %d to rank %d, received %d from rank %d\n",
           rank, send_val, dest, recv_val, src);

    MPI_Finalize();
    return 0;
}
