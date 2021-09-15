
#include <iostream>
#include <thread>
#include <cstdlib>
#include <cmath>

std::mutex mutex;
float trapeze_length;

//#Define DEBUG_BUILD

void show_help_info(char *program){
    std::cout << "Usage: " << program << " T N" << std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    std::cout << "N: the number of trapezes (int, N > 0)"<< std::endl;
    exit(1);
}

// target function
float get_func_res(float x){
    return 4/(1+x*x);
}

void trapeze_area(float x1, float x2, float& area){
    float f_x1 = get_func_res(x1);
    float f_x2 = get_func_res(x2);

    area = (f_x1 + f_x2) / 2 * std::abs(x2 - x1);
}

float cal_trapezes_per_thread(float start, int trapezes_per_thread, float trapeze_length){

    // float x1 = start;
    // float x2 = start + trapezes_per_size * trapeze_length;
    // for ...
    // cal trapeze_area
    for (int i = 0; i < trapezes_per_thread; i++){
        float x1 = start + i * trapezes_per_thread * trapeze_length;
        float x2 = x1 + trapeze_length;

        // add sum
    }
    
}

int main(int argc, char *argv[]){
    if (argc != 3){
        show_help_info(argv[0]);
    }

    // load and check the params
    int threads;
    int trapezes;    
    try{
        // the number of sub-threads
        threads = std::stoi(argv[1]) - 1;
        trapezes = std::stoi(argv[2]);
    } catch(std::exception) {
        show_help_info(argv[0]);
    }

    if (threads < 1 || trapezes < 1){
        show_help_info(argv[0]);
    }

    // create threads to execute a range of data
    float result = 0.0;
    std::thread *ni = new std::thread[threads]; 
    trapeze_length = 1.0f / trapezes;
    int trapezes_per_thread = trapezes / threads;
    int trapezes_remainder = trapezes - trapezes_per_thread * threads;
    
    int trapezes;
    float start = 0;
    for (int i = 0; i < threads; i++){
        
        // the number of trapezes to calculate for thread i
        if(trapezes_remainder > 0){
            trapezes = trapezes_per_thread + 1;
            --trapezes_remainder;
        }
        // 
        ni[i] = std::thread(cal_trapezes_per_thread, );
        // mutex.lock();
        // add result per thread
        // mutex.unlock();


        // coordinate for next thread
        start += trapezes * trapeze_length;
    }


    // main thread waits for all threads to finish
    for (int i = 0; i < threads; i++){
        ni[i].join();
    }
    std::cout << "The result of numberical intergration is: " << result << std::endl;
    delete[] ni;

    return 0;
}