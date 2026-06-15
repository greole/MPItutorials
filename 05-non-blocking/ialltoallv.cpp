#include <mpi.h>
#include <cstdio>
#include <vector>

// Same variable-count layout as 04-advanced-collectives/alltoallv.cpp,
// but initiated with MPI_Ialltoallv so local work can overlap the transfer.
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> sendcounts(size, rank + 1);
    std::vector<int> sdispls(size);
    sdispls[0] = 0;
    for (int j = 1; j < size; j++)
        sdispls[j] = sdispls[j-1] + sendcounts[j-1];

    int total_send = sdispls[size-1] + sendcounts[size-1];
    std::vector<int> sendbuf(total_send);
    for (int j = 0; j < size; j++)
        for (int k = 0; k < sendcounts[j]; k++)
            sendbuf[sdispls[j] + k] = rank * 100 + j * 10 + k;

    std::vector<int> recvcounts(size);
    std::vector<int> rdispls(size);
    for (int i = 0; i < size; i++)
        recvcounts[i] = i + 1;
    rdispls[0] = 0;
    for (int i = 1; i < size; i++)
        rdispls[i] = rdispls[i-1] + recvcounts[i-1];

    int total_recv = rdispls[size-1] + recvcounts[size-1];
    std::vector<int> recvbuf(total_recv);

    MPI_Request request;
    MPI_Ialltoallv(sendbuf.data(), sendcounts.data(), sdispls.data(), MPI_INT,
                   recvbuf.data(), recvcounts.data(), rdispls.data(), MPI_INT,
                   MPI_COMM_WORLD, &request);

    // Local work that runs while the transfer is in flight
    int local_sum = 0;
    for (int v : sendbuf)
        local_sum += v;
    printf("Rank %d: local send sum = %d (computed while transfer is in flight)\n",
           rank, local_sum);

    MPI_Wait(&request, MPI_STATUS_IGNORE);

    for (int i = 0; i < size; i++) {
        printf("Rank %d received from rank %d:", rank, i);
        for (int k = 0; k < recvcounts[i]; k++)
            printf(" %d", recvbuf[rdispls[i] + k]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
