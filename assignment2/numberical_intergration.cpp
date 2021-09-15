
#include <iostream>
#include <thread>
#include <cstdlib>
#include <cmath>
#include <mutex>

std::mutex mutex;
float trapeze_length; 
float all_area = 0; // the total area of all threads calculated

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

// calculate single trapeze_area
void trapeze_area(float x1, float x2, float& area){
    float f_x1 = get_func_res(x1);
    float f_x2 = get_func_res(x2);

    area += (f_x1 + f_x2) / 2 * std::abs(x2 - x1);
}


// calculate trapezes' area of a single thread
void cal_trapezes_per_thread(float start, int trapezes_per_thread){

    float area = 0.0;
    float x1, x2;

    for (int i = 0; i < trapezes_per_thread; i++){
        x1 = start + i * trapeze_length;
        x2 = x1 + trapeze_length;

        // cal trapeze_area
        trapeze_area(x1,x2,area);
    }
    
    // lock when adding to all_area 
    mutex.lock();
    all_area += area;
    mutex.unlock();
}

int main(int argc, char *argv[]){
    if (argc != 3){
        show_help_info(argv[0]);
    }

    // load and check the params
    int threads;
    int trapezes;    
    try{
        // the number of threads, main thread is concluded
        threads = std::stoi(argv[1]) - 1;
        trapezes = std::stoi(argv[2]);
    } catch(std::exception) {
        std::cout << "exception happens here" << std::endl;

        show_help_info(argv[0]);
    }

    if (threads < 1 || trapezes < 1){
        show_help_info(argv[0]);
    }

    // create threads to execute a range of data
    std::thread *ni = new std::thread[threads]; 
    trapeze_length = 1.0f / trapezes;
    int trapezes_per_thread = trapezes / threads;
    int trapezes_remainder = trapezes - trapezes_per_thread * threads;
    
    int trapezes_current;   // number of trapezes for current thread to calculate
    float start = 0;    // leftmost x coordinate for trapezes
    for (int i = 0; i < threads; i++){
        
        // the number of trapezes to calculate for thread i
        if(trapezes_remainder > 0){
            trapezes_current = trapezes_per_thread + 1;
            --trapezes_remainder;
        }
        // 
        ni[i] = std::thread(cal_trapezes_per_thread, start, trapezes_current);
        // coordinate for next thread
        start += trapezes_current * trapeze_length;
    }

    if(trapezes_remainder > 0){
        std::cout<<"Error: trapezes remainder is " << trapezes_remainder << std::endl;
    }

    // at last, calculate the main thread
    cal_trapezes_per_thread(start, trapezes_per_thread);


    // main thread waits for all threads to finish
    for (int i = 0; i < threads; i++){
        ni[i].join();
    }
    std::cout << "The result of numberical intergration is: " << all_area << std::endl;
    delete[] ni;

    return 0;
}