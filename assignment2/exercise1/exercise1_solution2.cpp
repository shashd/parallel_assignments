#include <iostream>
#include <thread>
#include <cstdlib>
#include <cmath>
#include <mutex>
#include <chrono>

// solution2 is different from the methods of distributing work among threads
// it trys to distribute the work evenly to every thread and let each thread have
// a task to execute.

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
float trapeze_area(float x1, float x2){
    float f_x1 = get_func_res(x1);
    float f_x2 = get_func_res(x2);

    return (f_x1 + f_x2) / 2.0 * std::abs(x2 - x1);
}


// calculate trapezes' area of a single thread
void cal_trapezes_per_thread(float start, float trapezes_per_thread){

    float area = 0.0;
    float x1, x2;

    int integer_segment = (int) trapezes_per_thread;
    float decimal = trapezes_per_thread - integer_segment;

    // decimal is bigger than zero
    if (fabs(decimal) > 1e-6){
        x1 = start;
        x2 = x1 + decimal * trapeze_length;
        area += trapeze_area(x1,x2);
    }
    // integer segment part
    if (integer_segment > 0){
        for (int i = 0; i < integer_segment; i++){
            x1 = start + i * trapeze_length;
            x2 = x1 + trapeze_length;

            // cal trapeze_area
            area += trapeze_area(x1,x2);
        }
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
        show_help_info(argv[0]);
    }

    if (threads < 0 || trapezes < 1){
        show_help_info(argv[0]);
    }
    // create threads to execute a range of data
    std::thread *ni = new std::thread[threads];

    trapeze_length = 1.0f / trapezes;
    float trapezes_per_thread;
    if (threads == 0){
        trapezes_per_thread = (float)trapezes;
    } else {
        trapezes_per_thread  = (float)trapezes / ((float)threads + 1.0);
    }
    float trapezes_remainder = trapezes - trapezes_per_thread * (threads + 1.0);
    float trapezes_current = trapezes_per_thread;   // number of trapezes for current thread to calculate
    float start;    // leftmost x coordinate for trapezes

    // *** timing begins here ***
    auto start_time = std::chrono::system_clock::now();
    for (int i = 0; i < threads; i++){

        // the number of trapezes to calculate for thread i
        if(trapezes_remainder > 0){
            if (trapezes_remainder < 1.0){
                trapezes_current = trapezes_current + trapezes_remainder;
                trapezes_remainder = 0.0;
            } else{
                trapezes_current = trapezes_current + 1.0;
                trapezes_remainder = trapezes_remainder - 1.0;
            }
        }
        // create sub threads to execute func
        ni[i] = std::thread(cal_trapezes_per_thread, start, trapezes_current);

        // coordinate for next thread
        mutex.lock();
        start += trapezes_current * trapeze_length;
        mutex.unlock();
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

    // *** timing ends here ***
    std::chrono::duration<double> duration =
            (std::chrono::system_clock::now() - start_time);
    std::cout << "Finished in " << duration.count() << " seconds (wall clock)." << std::endl;

    return 0;
}