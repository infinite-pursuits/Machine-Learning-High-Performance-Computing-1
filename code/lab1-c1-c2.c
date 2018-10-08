#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BILLION (unsigned int)1E9
#define ARRAY_SIZE (unsigned int)2E9
double array[ARRAY_SIZE];

void main_c1()
{
    int j =0;
    double sum=0;
    unsigned int i = 0;
    double elapsed_time_temp = 0;
    double elapsed_time = 1E20;
    struct timespec start, end;
    for(j=0; j<10 ;j++)
    {   sum = 0;
        clock_gettime(CLOCK_MONOTONIC,&start);
        for (i=0; i < ARRAY_SIZE; i++)
            sum += array[i]*2;
        clock_gettime(CLOCK_MONOTONIC,&end);
        elapsed_time_temp =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);
        if(elapsed_time_temp < elapsed_time)
            elapsed_time = elapsed_time_temp;

    }

    printf("*C1*");
    printf("\nSum : %lf", sum);
    printf("\nElapsed time : %lf seconds", elapsed_time);
    fflush(stdout);
}

void main_c1_c2(double *array)
{
    int j=0;
    double bw_c1 = 0;
    double bw_c2 = 0;
    double sum =0;
    double total_bytes=0;
    double flops_c1=0;
    double flops_c2=0;
    unsigned int i = 0;
    double elapsed_time_temp_c1 = 0;
    double elapsed_time_c1 = 1E20;
    double elapsed_time_temp_c2 = 0;
    double elapsed_time_c2 = 1E20;
    struct timespec start, end;
    for(j=0; j<10 ;j++)
    {   sum = 0;
        clock_gettime(CLOCK_MONOTONIC,&start);
        for (i=0; i < ARRAY_SIZE; i++)
            sum += array[i]*2;
        clock_gettime(CLOCK_MONOTONIC,&end);
        elapsed_time_temp_c1 =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);
        if(j==0)
            elapsed_time_c1 = elapsed_time_temp_c1;
        if(elapsed_time_temp_c1 < elapsed_time_c1)
            elapsed_time_c1 = elapsed_time_temp_c1;

    }
    total_bytes = (double)ARRAY_SIZE * (double)sizeof(double);
    bw_c1 = total_bytes/(elapsed_time_c1*(double)BILLION);
    flops_c1 = 2 * (double)ARRAY_SIZE/(elapsed_time_c1*(double)BILLION);

    printf("\n\n*C1*");
    printf("\nSum : %lf", sum);
    printf("\nTime : %lf secs", elapsed_time_c1);
    printf("\nBw : %lf GB/s", bw_c1);
    printf("\nFLOPS: %lf GFLOP/s", flops_c1);
    fflush(stdout);

    for(j=0; j<10; j++)
    {   sum = 0;
        clock_gettime(CLOCK_MONOTONIC,&start);
        for (i = 0; i < ARRAY_SIZE; i+=8)
        {
            sum +=array[i]*2;
            sum +=array[i+1]*2;
            sum +=array[i+2]*2;
            sum +=array[i+3]*2;
            sum +=array[i+4]*2;
            sum +=array[i+5]*2;
            sum +=array[i+6]*2;
            sum +=array[i+7]*2;
        }
        clock_gettime(CLOCK_MONOTONIC,&end);
        elapsed_time_temp_c2 =(end.tv_sec - start.tv_sec)+((end.tv_nsec - start.tv_nsec)/(double)BILLION);
        if(j==0)
            elapsed_time_c2 = elapsed_time_temp_c2;
        if(elapsed_time_temp_c2 < elapsed_time_c2)
            elapsed_time_c2 = elapsed_time_temp_c2;

    }


    bw_c2 = total_bytes/(elapsed_time_c2*(double)BILLION);
    flops_c2 = 2 * (double)ARRAY_SIZE/(elapsed_time_c2*(double)BILLION);

    /*printf("\n*C1* \nTime: %lf secs \nBw: %lf GB/s\nFLOPS: %lf GFLOP/s", elapsed_time_c1,bw_c1,flops_c1);
    fflush(stdout);

    printf("\n*C2* \nTime: %lf secs \nBw: %lf GB/s\nFLOPs: %lf GFLOP/s", elapsed_time_c2,bw_c2,flops_c2);
    fflush(stdout);*/
    printf("\n*C2*");
    printf("\nSum : %lf", sum);
    printf("\nTime : %lf secs", elapsed_time_c2);
    printf("\nBw : %lf GB/s", bw_c2);
    printf("\nFLOPS: %lf GFLOP/s", flops_c2);
    fflush(stdout);
}
int main()
{
    unsigned int i;
    for ( i = 0; i < ARRAY_SIZE; ++i)
        array[i] = (double)i/3;
    main_c1(array);
    main_c1_c2(array);
    return 0;
}
