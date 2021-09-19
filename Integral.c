#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

double func(double x){
    return x*x*x;
}

int main(int argc, char **argv){
    int task_id, numprocs;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double b=100,a=10;
    int n = atoi(argv[1]);
    double dx = (b-a)/n;

    double integral = 0;

    clock_t start, end;
    clock_t sys_start, sys_end;
    start = clock();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);

    if(task_id==0){
        double x;
        for (int i = 0; i < n; i++)
        {
            x = a + i*dx + dx/2;  
            MPI_Send(&x, 1, MPI_DOUBLE, i%(numprocs-1) + 1, 0, MPI_COMM_WORLD);
        }

        double local;
        for (int i = 1; i < numprocs; i++)
        {
            MPI_Status status;
            MPI_Recv(&local,1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,&status);
            integral += local;
        }
        printf("the approximate integral of x^3 from %.2f to %.2f equals %.4f\n",a,b,integral);
    }else{
        double x, local=0;
        for (int i = task_id-1; i < n; i+=numprocs-1)
        {
            MPI_Status status;
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            local += func(x) * dx;
        }
        MPI_Send(&local,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
    }
    end = clock();
    
    // 计时取最早开始时间和最晚结束时间
    MPI_Reduce(&start, &sys_start, 1, MPI_DOUBLE, MPI_MIN, 0 , MPI_COMM_WORLD);
    MPI_Reduce(&end, &sys_end, 1, MPI_DOUBLE, MPI_MAX, 0 , MPI_COMM_WORLD);
    
    MPI_Finalize();

    if(task_id==0)printf("%s running time: %.4f s\n", processor_name, (double)(sys_end-sys_start)/CLOCKS_PER_SEC);
    return 0;
}