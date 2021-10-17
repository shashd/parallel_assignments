#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <mpi.h>

#define FALSE 0
#define TRUE 1

int rank, pop;
int maximum, minimum, total;
int sqrt_max;
char *sieve;

void show_help_info(char *program){
    std::cout << "Usage: " << program << "N" << std::endl;
    std::cout << "N: the maximum positive interger (int, N >= 1)"<< std::endl;
    MPI_Finalize();
    exit(1);
}


// at the end, the rest marked with true are primes
void eratosthenes(int lowerbound, int upperbound) {
    for (int i = lowerbound; i * i <= upperbound; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= upperbound; j += i){
                sieve[j] = false;
            }
        }
    }
}

void eratosthenes_par(int lowerbound, int upperbound){
    // j is the loop index for flags[]
    for(int i = lowerbound; i <= upperbound; ++i){
        for(int q = 2; q <= sqrt_max; ++q){
            if(sieve[q] && i % q == 0){
                sieve[i] = FALSE;
                break;
            }
        }
    }
}

int set_bounds(int rank, int &lower, int &upper){
    // static variables will be initialized once and stay in the memory
    static int chunk = total / pop;
    static int remainder = total - chunk * pop;

    lower = minimum + rank * chunk;
    upper = lower + chunk - 1;
    if (rank < remainder){
        lower = lower + rank;
        upper = upper + rank + 1;
    }else{
        lower = lower + remainder;
        upper = upper + remainder;
    }
    return upper - lower + 1;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &pop);

    if (argc != 2){
        show_help_info(argv[0]);
    }

    // load and check the params
    try{
        maximum = atoi(argv[1]);
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (maximum < 1){
        show_help_info(argv[0]);
    }

    sqrt_max = (int)sqrt(maximum);
    // init sieve array
    sieve = new char[maximum + 1];
    memset(sieve, TRUE, sizeof(char) * (maximum + 1));
    sieve[0] = sieve[1] = FALSE;

    double t1 = MPI_Wtime();

    /*  compute primes up to sqrt_max in all processes, could also compute in
        only one process and send the result to others */

    minimum = sqrt_max + 1;
    total = maximum - minimum + 1;

    int lower, upper, clen;
    clen = set_bounds(rank, lower, upper);

    if (rank == 0){
        // execute serial part and bcast to other processors
        eratosthenes(2, sqrt_max);

        // send the sieve array
        MPI_Bcast(&sieve, sqrt_max+1, MPI_BYTE, 0, MPI_COMM_WORLD);

        eratosthenes_par(lower, upper, sieve);
        
        // reduces data
        MPI_Reduce(sieve, sieve, maximum + 1, MPI_BYTE, MPI_LAND, 0, MPI_COMM_WORLD);

    } else{
        // receive the sieve array
        MPI_Bcast(&sieve, sqrt_max+1, MPI_BYTE, 0, MPI_COMM_WORLD);
        eratosthenes_par(lower, upper, sieve);
        
        // reduce data
        MPI_Reduce(sieve, sieve, maximum + 1, MPI_BYTE, MPI_LAND, 0, MPI_COMM_WORLD);
    }
    

    double t2 = MPI_Wtime();

    std::cout << "\n\nPrimes: ";
    for(int i = 0; i <= sqrt_max; ++i)
        if(sieve[i])
            std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "Elapsed time: " << t2 - t1 << std::endl;
    
    delete[] sieve;
    MPI_Finalize();
    return 0;
}