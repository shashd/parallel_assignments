#include <iostream>
#include <cmath>
#include <cstring>
#include <mpi.h>

// marking the primes in this array, prime==true
bool* flags;
int sqrt_max;
#define MAX_SIZE 100

// at the end, the rest marked with true are primes
void eratosthenes(int lowerbound, int upperbound) {
    for (int i = lowerbound; i * i <= upperbound; ++i) {
        if (flags[i]) {
            for (int j = i * i; j <= upperbound; j += i){
                flags[j] = false;
            }
        }
    }
}


void eratosthenes_worker(int lowerbound, int upperbound) {
    for (int i = lowerbound; i <= upperbound; ++i) {
        for(int q = 2; q <= sqrt_max; ++q){
        	if(flags[q] && i % q == 0){
        		flags[i] = false;
        		break;
        	}
        }
    }
}

int main(int argc, char *argv[]) {

    // set maximum number fixed
    int maximum = 10000000;
    char name[MAX_SIZE]; // name of processor
    int rank, pop, p, len;

    /* no MPI calls before this point */
    MPI_Init(&argc, &argv);
    // the rank of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &pop);

    flags = new bool[maximum + 1];
    memset(flags, true, sizeof(bool) * (maximum + 1));
    flags[0] = flags[1] = false; // exclude 0 and 1
    sqrt_max = (int) std::sqrt(maximum); // round down

    int lower_bound = sqrt_max + 1;
    int upper_bound = maximum;

    int total = upper_bound - lower_bound + 1;
    int chunk = total / pop;
    int remainder = total - chunk * pop;

    // define msg for each processor
    bool msg[chunk+1];
    for (int i = 0; i = chunk+1; i++){
        msg[i] = false;
    }
    int index = lower_bound;

    // *** timing begins here ***
    double start = MPI_Wtime();
	// sequentially compute primes up to sqrt_max
    eratosthenes(2, sqrt_max); 

    // processor 0, receive msg from processors
    if (rank == 0){
        MPI_Get_processor_name(name, &len);
        printf("%s receive from process %d of %d\n", name, rank, pop);
        // receive from other processors
        for (p = 1; p < pop; p++){
            MPI_Recv(&msg, chunk+1, MPI_CXX_BOOL, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // combine msg
            for (int i = 0; i < chunk+1; i++){
                flags[index++] = msg[i];
            }
        }

    } else{
        // calculate and send msg
        MPI_Get_processor_name(name, &len);
        // get bounds by rank
        int lower = lower_bound + rank * chunk;
        int upper = lower + chunk - 1;
        if (rank + 1 <= remainder){
            lower = lower + rank;
            upper = upper + rank + 1;
        } else{
            lower = lower + remainder;
            upper = upper + remainder;
        }
        eratosthenes_worker(lower, upper);
        // send data segment
        for (int i = 0; i < chunk+1; i++){
            msg[i] = flags[lower+i];
        }
//        sprintf("Message from process %d at %s", rank, name);
        MPI_Send(&msg, chunk+2, MPI_CXX_BOOL, 0, 0, MPI_COMM_WORLD);
    }
    // *** timing ends here ***
    double end = MPI_Wtime();

    // output result 
    std::cout << "Primes: ";
    for (int i = 0, j = maximum + 1; i < j; i++){
       if (flags[i]){
           std::cout << i << " ";
       }
    }
    std::cout << std::endl;
    
    std::cout << "N= " << maximum << " finished in " << end - start << " seconds." << std::endl;
    
    MPI_Finalize();
    delete[] flags;
    return 0;
}