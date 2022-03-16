#include <iostream>
#include <mpi.h>
#include <Windows.h>
#include <time.h>

#define MCW MPI_COMM_WORLD
using namespace std;

int main(int argc, char** argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    srand(time(0) + rank);

    MPI_Finalize();
    return 0;
}