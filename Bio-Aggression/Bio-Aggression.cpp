#include <iostream>
#include <mpi.h>
#include <Windows.h>
#include <time.h>

#define MCW MPI_COMM_WORLD
using namespace std;

// 20 percent chance to attack another Beta. Increased likelihood with higher standing.
bool interaction(int rank, int standing) {
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

/*
Tags
    0 - Beta to Beta
    1 - Beta to Alpha
    2 - Beta to Female
    3 - Alpha to Beta
*/

int main(int argc, char** argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    srand(time(0) + rank);

    int flag1 = 0;   //Beta male interactions
    int flag2 = 0;  //Alpha Interaction
    int flag3 = 0;

    int whoAlpha = -1;
    int standing = 0;
    int attack = 0;
    int counter = 0;
    bool isAlpha = false;

    while (1) {
        // The female process
        if (!rank) {
        }

        // Male processes
        if (rank) {
            // Betas
            if (!isAlpha) {
                // Check for any incoming Beta attacks, increase standing if attacked and win
                MPI_Iprobe(MPI_ANY_SOURCE, 0, MCW, &flag1, MPI_STATUS_IGNORE);
                if (flag1 == 1 && standing < 5) {
                    MPI_Recv(&attack, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
                    if (attack < generateAttack(rank, standing)) {
                        standing += 1;
                        flag1 = 0;
                        attack = 0;
                    }
                }

                // Check for incoming Alpha attacks, increase standing if win, die if lose
                MPI_Iprobe(MPI_ANY_SOURCE, 3, MCW, &flag1, MPI_STATUS_IGNORE);
                if (flag1 == 1) {
                    MPI_Recv(&attack, 1, MPI_INT, isAlpha, 3, MCW, MPI_STATUS_IGNORE);
                    if (attack < generateAttack(rank, standing)) {
                        standing += 1;
                        flag1 = 0;
                        attack = 0;
                    }
                    else {
                        cout << "The Alpha killed off " << rank << "!!!";
                        break;
                    }
                }

                // Claim Alpha title or challenge current Alpha
                if (standing > 4) {
                    if (whoAlpha == -1) {
                        whoAlpha = rank;
                        cout << rank << " considers himself the new Alpha!";
                    }
                    else {
                        if (interaction(rank, standing)) {
                            attack = generateAttack(rank, standing);
                            MPI_Send(&attack, 1, MPI_INT, whoAlpha, 1, MCW);
                            cout << rank << " challenges the Alpha!"
                        }
                    }
                }
                


                if (quit) {
                    break;
                }

            }
            // Alpha
            else {
                
            }
                

        }
        // Female
        else {
        
        }

        // Alpha broadcasts who he is for everyone
        if (isAlpha) {
            MPI_Bcast(&whoAlpha, 1, MPI_INT, rank, MCW);
        }
    }
    MPI_Barrier(MCW);



    MPI_Finalize();
    return 0;
}