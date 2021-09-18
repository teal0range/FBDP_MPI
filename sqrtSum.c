#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MASTER 0



double *generateData(int len){
    double *array = (double *)malloc(sizeof(double) * len);
    for (int i = 0; i < len; i++)
    {
        array[i] = i*(i+1);
    }
    return array;
}

void print_array(double *array, int len){
    printf("[");
    for(int i=0;i<len;i++){
        printf("%.0f, ", array[i]);
    }
    printf("]\n");
}

int main(int argc, char **argv)
{
    int task_id, numprocs;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    int n = atoi(argv[1]);

    double *data = generateData(n);

    double sqrtSum = 0;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);

    double local = 0;

    for (int i = task_id; i < n ; i+=numprocs){
        if(data[i]==0)continue;
        local += sqrt(data[i]);
    }
    
    MPI_Reduce(&local, &sqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD);

    if(task_id==0){
        printf("the sqrt sum up to %d equals %.4f\n",n , sqrtSum);
    }

    MPI_Finalize();
    return 0;
}
