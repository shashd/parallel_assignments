# include <iostream>
# include <omp.h>
# include <sys/time.h>
/**
 * Use the OMP NUM THREADS environment variable to control the number of threads
 * Example: OMP_NUM_THREADS=2 ./case1.exe N
 * @param program
 */
void show_help_info(char *program){
    std::cout << "Usage: " << program << " T" << std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    exit(-1);
}

static int ** allocate_array(int N) {
    int ** array;
    array = (int **)malloc(N * sizeof(int*));
    for (int i = 0; i < N ; i++)
        array[i] = (int *)malloc(N * sizeof(int));
    return array;
}

static void init_array(int **array, int N, int value){
    for (int i = 0; i < N ; i++)
        for (int j = 0; j < N ; j++)
            array[i][j] = value;
}

static void free_array(int ** array, int N) {
    int i;
    for (i = 0 ; i < N ; i++)
        free(array[i]);
    free(array);
}

void print_matrix(int** c, int dim){
    std::cout <<"Matrix:" << std::endl;
    for(int i = 0; i < dim; ++i){
        for(int j = 0; j < dim; ++j)
            std::cout << c[i][j]<<" ";
        std::cout << std::endl;
    }
}

int main(int argc, char * argv[]){

    int threads_number = 0; // the number of threads
    int ** a, ** b, ** c; //arrays
    int dim = 40; // the length of the array
    double time;			//variables for timing
    struct timeval ts,tf;

    if (argc != 2){
        show_help_info(argv[0]);
    } else{
        dim = std::stoi(argv[1]);
    }

    a = allocate_array(dim);
    b = allocate_array(dim);
    c = allocate_array(dim);
    init_array(a,dim,1);
    init_array(b,dim,2);
    init_array(c,dim,0);

    gettimeofday(&ts,NULL);

    #pragma omp parallel default (none) shared(a,b,c,dim) num_threads(threads_number)
    {
        // case 3: all loops are parallelized
        # pragma omp for collapse(3)
        for (int i = 0; i < dim; i++){
            for (int j = 0; j < dim; j++){
                for (int k = 0; k < dim; k++){
                    //prevent multi-threads writing to c[i][j] at the same time
                    #pragma omp atomic
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
    gettimeofday(&tf,NULL);
    time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
    
//    print_matrix(a, dim);
//    print_matrix(b, dim);
//    print_matrix(c, dim);
    
    free_array(a, dim);
    free_array(b, dim);
    free_array(c, dim);
    std::cout << "Executed time: " << time << std::endl;

    return 0;
};