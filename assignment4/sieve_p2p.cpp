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
    std::cout << "Usage: " << program << "T N" << std::endl;
    //std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
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

void eratosthenes_par(int lowerbound, int upperbound, char flags[]){
    // j is the loop index for flags[]
    for(int i = lowerbound, j = 0; i <= upperbound; ++i, ++j){
        for(int q = 2; q <= sqrt_max; ++q){
            if(sieve[q] && i % q == 0){
                flags[j] = FALSE;
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
        // the number of threads, excluding main thread
        //threads = atoi(argv[1]);
        maximum = atoi(argv[1]);
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (maximum < 1){
        show_help_info(argv[0]);
    }

    sqrt_max = (int)sqrt(maximum);
    sieve = new char[sqrt_max + 1];
    memset(sieve, TRUE, sizeof(char) * (sqrt_max + 1));
    sieve[0] = sieve[1] = FALSE;

    double t1 = MPI_Wtime();

    /*  compute primes up to sqrt_max in all processes, could also compute in
        only one process and send the result to others */
    eratosthenes(2, sqrt_max); 
    
    minimum = sqrt_max + 1;
    total = maximum - minimum + 1;

    int lower, upper, clen;
    clen = set_bounds(rank, lower, upper);

    if(rank != 0){
        if(lower <= upper){
            char* flags = new char[clen];
            memset(flags, TRUE, sizeof(char) * clen);
            eratosthenes_par(lower, upper, flags);
            MPI_Send(flags, clen, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

            //std::cout << "Send " << lower << " " << upper <<"\n";
            delete []flags;
        }
    }else{
        char* flags = new char[total];
        memset(flags, TRUE, sizeof(char) * clen);
        eratosthenes_par(lower, upper, flags);

        for(int rank = 1, offset=clen; rank < pop; ++rank){
            clen = set_bounds(rank, lower, upper);
            if(lower <= upper){
                MPI_Recv(flags + offset, clen, MPI_BYTE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                offset += clen;
            }
        }

        double t2 = MPI_Wtime();

        std::cout << "\n\nPrimes: ";
        for(int i = 0; i <= sqrt_max; ++i)
            if(sieve[i])
                std::cout << i << " ";
        for (int i = 0; i < total; ++i)
            if (flags[i])
                std::cout << i + sqrt_max + 1 << " ";  
        std::cout << std::endl;
        std::cout << "Elapsed time: " << t2 - t1 << std::endl;
        delete []flags;
    }

    delete[] sieve;
    MPI_Finalize();
    return 0;
}