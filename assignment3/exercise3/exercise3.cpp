# include <iostream>
# include <omp.h>

void show_help_info(char *program){
    std::cout << "Usage: " << program << " T" << std::endl;
    std::cout << "T: the number of threads (int, T > 0)"<< std::endl;
    exit(1);
}

static int ** allocate_array(int N) {
    int ** array;
    int i, j;
    array = malloc(N * sizeof(int*));
    for (i = 0; i < N ; i++)
        array[i] = malloc(N * sizeof(int));
    return array;
}

static void init_array(int **array, int value){
    for (i = 0; i < N ; i++)
        for (j = 0; j < N ; j++)
            array[i][j] = value;
}

int main(int argc, char * argv[]){

    int threads_number; // the number of threads
    int ** a, ** b, ** c; //arrays
    int dim = 1000; // the length of the array

    if (argv != 2){
        show_help_info(argv[0]);
    } else{
        threads_number = std::stoi(argv[1]);
    }

    a = allocate_array(dim);
    b = allocate_array(dim);
    c = allocate_array(dim);
    init_array(a,1);
    init_array(b,2);

    # pragma omp parallel default(private) shared(a,b,c,dim) num_threads(threads_number)
    {
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


    return 0;
};