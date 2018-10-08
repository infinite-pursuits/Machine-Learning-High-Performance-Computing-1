#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mkl.h"
#define BILLION (unsigned int)1E9
#define C3 (double)159.56710474507418 

double** init(int rows, int cols)
{   int i = 0;
    double ** matrix = (double **)malloc(rows * sizeof(double *));
    for(; i<rows ; i++)
       matrix[i] = (double *)malloc(cols * sizeof(double));
    for(i=0; i<rows; i++)
        for(int j = 0; j<cols ; j++)
            matrix[i][j] = 0.4+((double)(((i+j) % 40 )- 20.0)/40.0);
    return matrix;
}

double** init_zeros(int rows, int cols)
{   int i = 0;
    double ** matrix = (double **)malloc(rows * sizeof(double *));
    for(; i<rows ; i++)
       matrix[i] = (double *)malloc(cols * sizeof(double));
    for(i=0; i<rows; i++)
        for(int j = 0; j<cols ; j++)
            matrix[i][j] = 0;
    return matrix;
}

double** flatten(double **matrix, int rows, int cols)
{
    double** flattened_mat = init_zeros(rows*cols,1);
    int k = 0;
    for(int i=0; i<rows ; i++)
        for(int j=0; j<cols ; j++)
            flattened_mat[k++][0] = matrix[i][j];
    return flattened_mat;
}

double** multiply(double **mat1, double **mat2, int row2, int col2, double *elapsed_time_mul)
{
    double** mat_resultant = init_zeros(1, col2);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC,&start);
    for(int i=0; i<col2; i++)
        for(int j=0; j<row2; j++)
            mat_resultant[0][i] += mat1[j][0] * mat2[j][i];
    clock_gettime(CLOCK_MONOTONIC,&end);
    *elapsed_time_mul =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);
    return mat_resultant;
}

double** relu(double **mat, int cols, double* elapsed_time_rel)
{   int i = 0;
    double** rel_mat = init_zeros(1, cols);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(; i<cols;i++)
    {
        if (mat[0][i]<0)
            rel_mat[0][i] = 0;
        else
            rel_mat[0][i] = mat[0][i];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    *elapsed_time_rel =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);
    return rel_mat;
}

void freeing(double** matrix, int rows)
{
    for(int i=0;i<rows; i++)
        free(matrix[i]);
}

void c5()
{
    double elapsed_time_mul1, elapsed_time_mul2, elapsed_time_relu1, elapsed_time_relu2,elapsed_time;
    int i=0;
    double sum =0.0;

    double** x0 = init(256, 256);
    double** w0 = init(65536, 4000);
    double** w1 = init(4000, 1000);

    double** flattened_x0 = flatten(x0, 256, 256);
    double** intermediate = multiply(flattened_x0, w0, 65536, 4000, &elapsed_time_mul1);
    double** z0 = relu(intermediate, 4000, &elapsed_time_relu1);
    double** flattened_z0 = flatten(z0, 1, 4000);
    double** intermediate2 = multiply(flattened_z0, w1, 4000, 1000, &elapsed_time_mul2);
    double** z1 = relu(intermediate2, 1000, &elapsed_time_relu2);

    for(; i<1000; i++)
       sum += z1[0][i];
    elapsed_time = elapsed_time_mul1+elapsed_time_mul2+elapsed_time_relu1+elapsed_time_relu2;
    printf("*C5*\nElapsed_time: %lf secs\nSpeedup wrt C3: %lf times\nSum is: %lf", elapsed_time, C3/elapsed_time,sum);
    fflush(stdout);

    freeing(x0, 256);
    freeing(w0, 65536);
    freeing(w1, 4000);
    freeing(intermediate, 1);
    freeing(z0, 1);
    freeing(flattened_z0, 4000);
    freeing(intermediate2, 1);
    freeing(flattened_x0, 65536);
    freeing(z1, 1);
}

void init_c6(double * mat, int rows, int cols)
{
    for(int i=0;i<rows;i++)
    {   for(int j=0; j<cols; j++)
           mat[i*cols+j] = 0.4+((double)(((i+j) % 40 )- 20.0)/40.0);
    }
}

void relu_c6(double *z, int rows)
{
 for(int i=0;i<rows;i++)
    {
        if (z[i]<0)
            z[i]=0;
        else
            z[i]= z[i];

    }
}
void c6()
{
        double elapsed_time;
        double sum =0.0;
        struct timespec start, end;

        double* x0 = (double *) mkl_malloc(256 * 256 * sizeof(double),64);
        double* w0 = (double *) mkl_malloc(65536 * 4000 * sizeof(double),64);
        double* w1 = (double *) mkl_malloc(4000 * 1000 * sizeof(double),64);
        double* z0 = (double *) mkl_malloc(4000 * sizeof(double), 64);
        double* z1 = (double *) mkl_malloc(1000 * sizeof(double), 64);

        init_c6(x0,256,256);
        init_c6(w0,65536,4000);
        init_c6(w1,4000,1000);

        clock_gettime(CLOCK_MONOTONIC,&start);
        cblas_dgemv (CblasRowMajor, CblasTrans, 65536, 4000,
        1.0, w0, 4000, x0, 1, 0.0, z0, 1);

        relu_c6(z0, 4000);

        cblas_dgemv (CblasRowMajor, CblasTrans, 4000, 1000,
        1.0, w1, 1000, z0, 1, 0.0, z1, 1);

        relu_c6(z1, 1000);

        clock_gettime(CLOCK_MONOTONIC,&end);

        elapsed_time =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);

        for(int i=0; i<1000; i++)
           sum += z1[i];

        printf("\n*C6*\nElapsed_time: %lf secs\nSpeedup wrt C3: %lf times\nSum is: %lf", elapsed_time, C3/elapsed_time ,sum);
        fflush(stdout);

        mkl_free(x0);
        mkl_free(w0);
        mkl_free(w1);
        mkl_free(z0);
        mkl_free(z1);
}

int main()
{
    c5();
    c6();
    return 0;
}
