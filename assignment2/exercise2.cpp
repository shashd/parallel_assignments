#include <iostream>
#include <pthread.h>
#include <vector>
#include <cmath>

struct worker_info{
    int lowerbound;
    int upperbound;
    std::vector<bool> *flag;
};

void show_help_info(char *program){
    std::cout << "Usage: " << program << "M P" << std::endl;
    std::cout << "M: the maximum positive interger (int, M > 1)"<< std::endl;
    std::cout << "P: the number of threads (int, P > 0)"<< std::endl;
    exit(1);
}


// todo: modify
// at the end, the rest marked with true are primes
void eratosthenes(int lowerbound, int upperbound, std::vector<bool> &flag) {
    for (int i = lowerbound; i * i <= upperbound; ++i) {
        if (flag[i]) {
            for (int j = i * i; j <= upperbound; j += i){
                flag[j] = false;
            }
        }
    }	
}

void *worker(void *arg){
    // worker_info *wi = (static_cast<worker_info*>) arg;
    
    worker_info *wi = (struct worker_info *) arg;
    eratosthenes(wi->lowerbound, wi->upperbound, *(wi->flag));
    return nullptr;
}

int main(int argc, char *argv[]) {

    if (argc != 3){
        show_help_info(argv[0]);
    }

    // load and check the params
    int maximum;
    int threads;    
    try{
        maximum = std::stoi(argv[1]);
        
        // the number of threads, main thread is excluded
        threads = std::stoi(argv[2]) - 1;
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (threads < 0 || maximum < 1){
        show_help_info(argv[0]);
    }

    // init vector's size and value
    std::vector<bool> flag(maximum + 1, true);
    flag[0] = flag[1] = false; //exclude 0 and 1


    // sequentially compute primes up to sqrt_max
    
    int sqrt_max = (int) std::sqrt(maximum);    // round down
    eratosthenes(2, sqrt_max, flag);

    // multi-threads part
    int lower_bound = sqrt_max + 1;
    int upper_bound = maximum;
    int len = upper_bound - lower_bound + 1;
    int chunk_length = len / (threads + 1);
    // todo: deal with chunk remainder
    int chunk_remainder = len - (threads + 1) * chunk_length;
    
    // stores return value from pthread_create
    int rc;

    // create sub-threads identifier
    pthread_t et[threads];
    //  
    struct worker_info wi[threads];
    int i;
    for (i = 0; i < threads; i++){
        std::cout << "Create thread " << i << std::endl;


        // todo: calculate lowerbound and upperbound
        // wi[i].lowerbound = lower_bound;
        // wi[i].upperbound = lower_bound + chunk_length - 1;
        // wi[i].flag = &flag;
        wi[i] = { lower_bound, lower_bound + chunk_length - 1, &flag };
        lower_bound += chunk_length;

        // create pthreads to execute tasks
        rc = pthread_create(&et[i], NULL, worker, (void *)&wi[i]);
        // fail to create thread
        if (rc!=0){
            std::cout<<"Error: fail to create threads, and code returned from pthread_create is: " << rc << std::endl;
            exit(-1);
        }
    }
    
    // main thread execute
    eratosthenes(lower_bound, maximum, flag);

    for(i = 0; i < threads; ++i)
        pthread_join(et[i], NULL);
    
    //pthread_exit(NULL);

    // output result 
    for (int i = 0; i < maximum+1; i++){
        if (flag[i]){
            std::cout << i << " " ;
        }
    }

    return 0;
}