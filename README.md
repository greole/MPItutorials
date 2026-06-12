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
└── 02-collectives/
    ├── broadcast.cpp          # MPI_Bcast: root rank broadcasts its rank to all
    └── broadcast_reduce.cpp   # MPI_Bcast + MPI_Reduce: broadcast then sum to rank 0
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
