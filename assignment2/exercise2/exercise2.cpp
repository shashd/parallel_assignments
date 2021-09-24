#include <iostream>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <cstring>
#include <chrono>

// marking the primes in this array, prime==true
bool* flags;

int sqrt_max;

struct worker_info{
    int id;	// thread identifier, just for debug
    int lowerbound;
    int upperbound;
};

void show_help_info(char *program){
    std::cout << "Usage: " << program << "T N" << std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    std::cout << "N: the maximum positive interger (int, N >= 1)"<< std::endl;
    exit(1);
}


// todo: modify
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

void* worker(void *arg){
    worker_info *wi = static_cast<worker_info*>(arg);

    //std::cout << "Thread: " << wi->id << std::endl;

    eratosthenes_worker(wi->lowerbound, wi->upperbound);
    return nullptr;
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
        threads = std::stoi(argv[1]) - 1;
        maximum = std::stoi(argv[2]);
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (threads < 0 || maximum < 1){
        show_help_info(argv[0]);
    }

    flags = new bool[maximum + 1];
    memset(flags, true, sizeof(bool) * (maximum + 1));
    flags[0] = flags[1] = false; // exclude 0 and 1

    sqrt_max = (int) std::sqrt(maximum); // round down

   	// multi-threads part
    int lower_bound = sqrt_max + 1;
    int upper_bound = maximum;
    int len = upper_bound - lower_bound + 1;
    int chunk_length = len / (threads + 1);
    
    int chunk_remainder = len - (threads + 1) * chunk_length;
    // stores return value from pthread_create
    int rc;
    // create sub-threads identifier
    pthread_t et[threads];
    
    struct worker_info wi[threads];

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();
	// sequentially compute primes up to sqrt_max
    eratosthenes(2, sqrt_max); 
    for (int i = 0, cl; i < threads; i++){
    
    	if(chunk_remainder > 0){
    		cl = chunk_length + 1;
    		--chunk_remainder;
    	}else{
    		cl = chunk_length;
    	}
    
        wi[i] = { i, lower_bound, lower_bound + cl - 1 };
        lower_bound += cl;

        // create pthreads to execute tasks
        rc = pthread_create(&et[i], NULL, worker, (void *)&wi[i]);
        // fail to create thread
        if (rc!=0){
            std::cout<<"Error: fail to create threads, and code returned from pthread_create is: " << rc << std::endl;
            exit(-1);
        }
    }
    
    // main thread execute
    eratosthenes_worker(lower_bound, maximum);

    for(int i = 0; i < threads; ++i)
        pthread_join(et[i], NULL);

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
    
    std::cout << "Finished in " << duration.count() << " seconds (wall clock)." << std::endl;
    
    delete[] flags;
    return 0;
}
