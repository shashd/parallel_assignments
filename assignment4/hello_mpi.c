#include <string.h>
#include <stdio.h>
#include <mpi.h>

#define MAX_SIZE 100

int main(int argc, char *argv[])
{
    char msg[MAX_SIZE], name[MAX_SIZE];
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int rank, size, p, len;

    /* no MPI calls before this point */
    MPI_Init(&argc, &argv);
    // process number
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // process size
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // get process name
    MPI_Get_processor_name(processor_name,&len);
    // rank = 0, only receive
    if (rank == 0) {
        printf("Hello from process %d of %d on %s\n", rank, size, processor_name);
        for (p = 1; p < size; p++) {
	        MPI_Recv(msg, MAX_SIZE, MPI_CHAR, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	        printf("%s\n", msg);
        }
    } else {
        printf(msg, "Message from process %d at %s on %s", rank, name, processor_name);
        MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    /* no MPI calls after this point */
    return 0;
}
