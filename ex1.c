#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int rank, size, code, tag = 100;
    int left_proc, right_proc, local_value , temp = 0, sum = 0;
    MPI_Status status;
    MPI_Request send_request, recv_request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
    local_value = rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    left_proc = rank - 1;
    right_proc = rank + 1;

    if (rank == 0)         left_proc = size - 1;
    if (rank == size-1)    right_proc = 0;
    
    while (1) {
        MPI_Issend(&local_value, 1, MPI_INTEGER, right_proc, tag, MPI_COMM_WORLD, &send_request);
        MPI_Recv(&temp, 1, MPI_INTEGER, left_proc, tag, MPI_COMM_WORLD, &status);
        MPI_Wait(&send_request, &status);
        local_value = temp;
        sum += local_value;
        if (local_value == rank) break;
    }
    
    printf("Process %d, Sum = %d\n", rank, sum);

    MPI_Finalize();

    return 0;
}