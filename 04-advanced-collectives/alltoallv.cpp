#include <mpi.h>
#include <cstdio>
#include <vector>

// Each rank i sends (i+1) integers to every other rank.
// The sent value to rank j is: rank*100 + j*10 + element_index.
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // sendcounts[j] = how many ints rank sends to rank j
    std::vector<int> sendcounts(size);
    std::vector<int> sdispls(size);
    for (int j = 0; j < size; j++)
        sendcounts[j] = rank + 1;

    sdispls[0] = 0;
    for (int j = 1; j < size; j++)
        sdispls[j] = sdispls[j-1] + sendcounts[j-1];

    int total_send = sdispls[size-1] + sendcounts[size-1];
    std::vector<int> sendbuf(total_send);
    for (int j = 0; j < size; j++)
        for (int k = 0; k < sendcounts[j]; k++)
            sendbuf[sdispls[j] + k] = rank * 100 + j * 10 + k;

    // recvcounts[i] = how many ints rank receives from rank i = i+1
    std::vector<int> recvcounts(size);
    std::vector<int> rdispls(size);
    for (int i = 0; i < size; i++)
        recvcounts[i] = i + 1;

    rdispls[0] = 0;
    for (int i = 1; i < size; i++)
        rdispls[i] = rdispls[i-1] + recvcounts[i-1];

    int total_recv = rdispls[size-1] + recvcounts[size-1];
    std::vector<int> recvbuf(total_recv);

    MPI_Alltoallv(sendbuf.data(), sendcounts.data(), sdispls.data(), MPI_INT,
                  recvbuf.data(), recvcounts.data(), rdispls.data(), MPI_INT,
                  MPI_COMM_WORLD);

    for (int i = 0; i < size; i++) {
        printf("Rank %d received from rank %d:", rank, i);
        for (int k = 0; k < recvcounts[i]; k++)
            printf(" %d", recvbuf[rdispls[i] + k]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
