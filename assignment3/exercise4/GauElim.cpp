#include <iostream>
#include <omp.h>

#ifdef TEST1
#define N 3 
float a[3][3] = {{2,-3, 0},
                 {0, 1, 1},
                 {0, 0,-5}};
    
float b[3] = {3,1,0};

float x[3];

#elif defined TEST2
#define N 3 
float a[3][3] = {{2,  1,  -1},
                 {0,0.5f,0.5f},
                 {0,  0,  -1}};
    
float b[3] = {8,1,1};

float x[3];

#elif defined TEST3
#define N 4
float a[4][4] = {{1,1,   -3,      1},
                 {0,8,  -19,      6},
                 {0,0,2.625f, -1.25f},
                 {0,0,    0, 0.8095f}};
float b[4] = {2,10,0.25f,8.2381f};
//correct approximate solution {1.2941, 5.3529, 4.9412, 10.176}
float x[4];
#endif

void row_oriented_solver(){
    int row, col;
    for(row = N-1; row >= 0; --row){
        x[row] = b[row];
        for(col=row+1; col < N; ++col)
            x[row] -= a[row][col] * x[col];
        x[row] /= a[row][row];
    }
}

void column_oriented_solver(){
    int row, col;

    #pragma omp parallel private(col)
    {
        #pragma omp for
        for(row = 0; row < N; ++row)
            x[row] = b[row];
        //implicit barrier
        for(col = N-1; col >= 0; --col){
            #pragma omp single
            x[col] /= a[col][col];
            //implicit barrier
            #pragma omp for
            for(row = 0; row < col; ++row)
                x[row] -= a[row][col] * x[col];
        }
    }
}

int main(){
    //row_oriented_solver();
    column_oriented_solver();

    for(int i = 0; i < N; ++i)
        printf("x%d=%f\n", i, x[i]);
    
    return 0;
}