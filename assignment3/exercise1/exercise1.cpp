#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>
#include <omp.h>

// marking the primes in this array, prime==true
bool* flags;

int sqrt_max;

void show_help_info(char *program){
    std::cout << "Usage: " << program << "T N" << std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    std::cout << "N: the maximum positive interger (int, N >= 1)"<< std::endl;
    exit(1);
}


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

int main(int argc, char *argv[]) {
    if (argc != 3){
        show_help_info(argv[0]);
    }

    // load and check the params
    int threads;    
    int maximum;
    try{
        // the number of threads, excluding main thread
        threads = std::stoi(argv[1]);
        maximum = std::stoi(argv[2]);
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (threads < 1 || maximum < 1){
        show_help_info(argv[0]);
    }

    flags = new bool[maximum + 1];
    memset(flags, true, sizeof(bool) * (maximum + 1));
    flags[0] = flags[1] = false; // exclude 0 and 1

    sqrt_max = (int) std::sqrt(maximum); // round down

    // ============== multi-threads part ================
    int lower_bound = sqrt_max + 1;
    int upper_bound = maximum;

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();
	// sequentially compute primes up to sqrt_max
    eratosthenes(2, sqrt_max); 
    
    // set the number of threads
    omp_set_num_threads(threads);
    #pragma omp parallel for
    for(int i = lower_bound; i <= upper_bound; ++i){
        // check if it's prime
        for(int q = 2; q <= sqrt_max; ++q){
            if(flags[q] && i % q == 0){
                flags[i] = false;
                break;
            }
        }
    }
    
	// *** timing ends here ***
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start_time;

    // output result 
   std::cout << "Primes: ";
   for (int i = 0, j = maximum + 1; i < j; i++){
      if (flags[i]){
          std::cout << i << " ";
      }
   }
   std::cout << std::endl;
    
    std::cout << "T= " << threads << ", N= " << maximum << " finished in " << duration.count() << " seconds (wall clock)." << std::endl;
    
    delete[] flags;
    return 0;
}