# include <iostream>
# include <omp.h>
# include <sys/time.h>

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
int main(int argc, char * argv[]){

    int threads_number; // the number of threads
    int ** a, ** b, ** c; //arrays
    int dim = 1000; // the length of the array
    double time;			//variables for timing
    struct timeval ts,tf;

    if (argc != 2){
        show_help_info(argv[0]);
    } else{
        threads_number = std::stoi(argv[1]);
    }

    a = allocate_array(dim);
    b = allocate_array(dim);
    c = allocate_array(dim);
    init_array(a,dim,1);
    init_array(b,dim,2);

    gettimeofday(&ts,NULL);

    # pragma omp parallel default (none) shared(a,b,c,dim) num_threads(threads_number)
    {
        // case 1: outermost loop is parallelized
        # pragma omp for schedule(static)
        for (int i = 0; i < dim; i++){
            for (int j = 0; j < dim; j++){
                c[i][j] = 0;
                for (int k = 0; k < dim; k++){
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    gettimeofday(&tf,NULL);
    time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
    free_array(a, dim);
    free_array(b, dim);
    free_array(c, dim);
    std::cout << "Executed time: " << time << std::endl;

    return 0;
};