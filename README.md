# MPI Tutorials

A collection of MPI examples written in C++, organized by topic.

## Prerequisites

- C++17-capable compiler (GCC, Clang, MSVC)
- MPI implementation (e.g. [OpenMPI](https://www.open-mpi.org/), [MPICH](https://www.mpich.org/))
- CMake 3.10+

On macOS with Homebrew:
```sh
brew install open-mpi cmake
```

On Ubuntu/Debian:
```sh
sudo apt install libopenmpi-dev cmake
```

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Project Structure

```
MPItutorials/
├── CMakeLists.txt
├── 01-hello-world/
│   └── hello-world.cpp       # Each rank prints its rank and total size
├── 02-collectives/
│   ├── broadcast.cpp          # MPI_Bcast: root rank broadcasts its rank to all
│   └── broadcast_reduce.cpp   # MPI_Bcast + MPI_Reduce: broadcast then sum to rank 0
├── 03-point-to-point/
│   ├── send_recv.cpp           # MPI_Send/MPI_Recv: rank 0 sends a value to rank 1
│   ├── ping_pong.cpp           # Blocking send/recv round-trips between rank 0 and 1
│   └── ring.cpp                # MPI_Sendrecv: each rank passes its rank around a ring
├── 04-advanced-collectives/
│   ├── alltoallv.cpp           # MPI_Alltoallv: rank i sends (i+1) ints to every rank
│   └── alltoallv_gather.cpp    # MPI_Alltoall + MPI_Alltoallv: redistribute by (e % size)
└── 05-non-blocking/
    ├── isend_irecv.cpp         # MPI_Isend/MPI_Irecv + MPI_Waitall: ring pass with overlap
    └── ialltoallv.cpp          # MPI_Ialltoallv + MPI_Wait: alltoallv with overlap
```

## Examples

### 01 — Hello World

Each rank prints `Hello world from rank i/n`.

```sh
mpirun -n 4 build/01-hello-world/hello-world
```

### 02 — Collectives

#### Broadcast

The specified root rank broadcasts its rank number to all other ranks. Each rank prints the received value.

```sh
mpirun -n 4 build/02-collectives/broadcast <root_rank>
```

#### Broadcast + Reduce

Same broadcast as above, followed by an `MPI_Reduce` that sums all received values to rank 0, which then prints the result.

```sh
mpirun -n 4 build/02-collectives/broadcast_reduce <root_rank>
```

### 03 — Point-to-Point

#### Send / Recv

Rank 0 sends the integer `42` to rank 1 using blocking `MPI_Send`/`MPI_Recv`.

```sh
mpirun -n 2 build/03-point-to-point/send_recv
```

#### Ping-Pong

Rank 0 and rank 1 exchange a counter back and forth for 4 rounds. Rank 1 increments the value on each bounce.

```sh
mpirun -n 2 build/03-point-to-point/ping_pong
```

#### Ring

Each rank sends its own rank to the next process and receives from the previous using `MPI_Sendrecv`, passing a token once around the ring.

```sh
mpirun -n 4 build/03-point-to-point/ring
```

### 04 — Advanced Collectives

#### Alltoallv

Rank i sends `(i+1)` integers to every other rank (variable send counts). Each value encodes its origin and destination: `rank*100 + dest*10 + element_index`.

```sh
mpirun -n 4 build/04-advanced-collectives/alltoallv
```

#### Alltoallv — Redistribute by modulo

Rank i starts with `(i+1)` consecutive integers. `MPI_Alltoall` exchanges the per-rank send counts; `MPI_Alltoallv` redistributes the data so that element `e` ends up on rank `e % size`.

```sh
mpirun -n 4 build/04-advanced-collectives/alltoallv_gather
```

### 05 — Non-Blocking

#### Isend / Irecv

Non-blocking ring pass: each rank posts `MPI_Irecv` and `MPI_Isend`, does local work while messages are in flight, then calls `MPI_Waitall`.

```sh
mpirun -n 4 build/05-non-blocking/isend_irecv
```

#### Ialltoallv

Same variable-count layout as `04-advanced-collectives/alltoallv`, but initiated with `MPI_Ialltoallv`. Local work runs between the init call and `MPI_Wait`; the interleaved output shows the overlap.

```sh
mpirun -n 4 build/05-non-blocking/ialltoallv
```
