#include <iostream>
#include <mpi.h>
#include <Windows.h>
#include <time.h>

#define MCW MPI_COMM_WORLD
using namespace std;

// 20 percent chance to attack another Beta. Increased likelihood with higher standing.
bool betaInteraction(int rank, int standing) {
    int interactionChance = ((rand() + rank) % 10) + ceil(standing / 2);
    if (interactionChance >= 8) { return true; }
    else { return false; }
}

//only 1/7 chance to interact with female. Decreases further with lower standing.
bool femaleInteraction(int rank, int standing) {
    int interactionChance = ((rand() + rank) % 21) - ceil(5/standing);
    if (interactionChance >= 18) { return true; }
    else { return false; }

}

// Generate attack value from 1 - 50. The higher the standing, the greater the advantage.
// Alpha (low, high) = (25, 74)     Beta 0 = (1,51)
int generateAttack(int rank, int standing) {
    int power = ((rand() + rank) % 50) + pow(standing, 2);
    return power;
}


int main(int argc, char** argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    srand(time(0) + rank);

    // The female process
    if (!rank) {

    }

    // Male processes
    if (rank) {
        int standing = 0;
        bool isAlpha = false;

    }

    MPI_Finalize();
    return 0;
}