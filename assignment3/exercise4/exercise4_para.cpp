# include <iostream>
# include <omp.h>
#include <chrono>

void show_help_info(char *program){
    std::cout << "Usage: " << program << " N T" << std::endl;
    std::cout << "N: the number of x (int, N > 0)"<< std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    exit(-1);
}

void row_oriented(int n, int **A, int *x, int *b){
    for (int row = n - 1; row >= 0; row--){
        x[row] = b[row];
        for (int col = row+1; col < n; col++){
            x[row] -= A[row][col] * x[col];
        }
        x[row] /= A[row][row];
    }
}

void column_oriented(int n, int **A, int *x, int *b){

    #pragma omp parallel private(col)
    {
        #pragma omp for
        for (int row = 0; row < n; row++){
            x[row] = b[row];
        }
        //implicit barrier
        for (int col = n-1; col >= 0; col--){
            #pragma omp single
            x[col] /= A[col][col];
            //implicit barrier
            #pragma omp for
            for (int row = 0; row < col; row++){
                x[row] -= A[row][col] * x[col];
            }
        }
    }
}

int ** init_A(int N) {
    int ** array;
    // allocate space
    array = (int **)malloc(N * sizeof(int*));
    for (int i = 0; i < N ; i++)
        array[i] = (int *)malloc(N * sizeof(int));
    // init upper triangular value
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (j >= i){
                array[i][j] = 1;
            } else{
                array[i][j] = 0;
            }
        }
    }
    return array;
}

int * init_B(int N){
    int *b = (int *)malloc(N * sizeof(int*));
    int temp = N;
    // set b from n to 1
    for (int i = 0; i < N; i++){
        b[i] = temp--;
    }
    return b;
}

bool validate_solution(int *x, int N){
    for (int i = 0; i < N; i++){
        if (x[i]!=1){
            return false;
        }
    }
    return true;
}

void free_two_dim_array(int **A, int N){
    for (int i = 0; i < N; i++){
        delete[] A[i];
    }
    delete[] A;
}


int main(int argc, char * argv[]) {

    int n; // the number of x
    int threads_number; // the number of thread

    if (argc != 3){
        show_help_info(argv[0]);
    } else{
        n = std::stoi(argv[1]);
        threads_number = std::stoi(argv[2]);
    }

    int *b = init_B(n);; // the equation value array
    int ** A = init_A(n); // the coefficients array of x
    int *x = new int[n]; // the solution array

    // compute the solution
    auto start_time = std::chrono::system_clock::now();
    //row_oriented(n,A,x,b);
    column_oriented(n,A,x,b);
    std::chrono::duration<double> duration = std::chrono::system_clock::now() - start_time;

    // show answer
    std::cout << "T= " << threads_number << ", N= " << n << " finished in " << duration.count() << " seconds (wall clock)." << std::endl;
    if (validate_solution(x,n)){
        std::cout << "The solution is correct." << std::endl;
    }
    // free spaces
    free_two_dim_array(A,n);
    delete[] b;
    delete[] x;

    return 0;
};
