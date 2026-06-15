#include <mpi.h>
#include <cstdio>
#include <vector>

// Redistribute a globally uneven array so that element e lands on rank (e % size).
// Rank i starts with (i+1) consecutive integers: { i*(i+1)/2, ..., i*(i+1)/2 + i }.
// MPI_Alltoall first exchanges the per-rank send counts; MPI_Alltoallv moves the data.
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_size  = rank + 1;
    int local_start = rank * (rank + 1) / 2;

    std::vector<int> local(local_size);
    for (int k = 0; k < local_size; k++)
        local[k] = local_start + k;

    // Count how many local elements go to each destination rank
    std::vector<int> sendcounts(size, 0);
    for (int k = 0; k < local_size; k++)
        sendcounts[local[k] % size]++;

    std::vector<int> sdispls(size);
    sdispls[0] = 0;
    for (int j = 1; j < size; j++)
        sdispls[j] = sdispls[j-1] + sendcounts[j-1];

    // Group send buffer by destination rank
    std::vector<int> sendbuf(local_size);
    std::vector<int> pos(sdispls.begin(), sdispls.end());
    for (int k = 0; k < local_size; k++) {
        int dest = local[k] % size;
        sendbuf[pos[dest]++] = local[k];
    }

    // Let every rank learn how many elements it will receive from each sender
    std::vector<int> recvcounts(size);
    MPI_Alltoall(sendcounts.data(), 1, MPI_INT,
                 recvcounts.data(), 1, MPI_INT,
                 MPI_COMM_WORLD);

    std::vector<int> rdispls(size);
    rdispls[0] = 0;
    for (int i = 1; i < size; i++)
        rdispls[i] = rdispls[i-1] + recvcounts[i-1];

    int total_recv = rdispls[size-1] + recvcounts[size-1];
    std::vector<int> recvbuf(total_recv);

    MPI_Alltoallv(sendbuf.data(),  sendcounts.data(), sdispls.data(), MPI_INT,
                  recvbuf.data(),  recvcounts.data(), rdispls.data(), MPI_INT,
                  MPI_COMM_WORLD);

    printf("Rank %d owns elements (e %% %d == %d):", rank, size, rank);
    for (int v : recvbuf)
        printf(" %d", v);
    printf("\n");

    MPI_Finalize();
    return 0;
}
