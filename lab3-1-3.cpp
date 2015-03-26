#include <mpi.h>
#include <iostream>

// Кількість створюваних процесів - 6
#define COUNT_PROCESS 6

int main(int argc, char **argv)
{
	int rank;
	int tag = 0;
	MPI_Status status;
	MPI_Request request;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0){
		// формування стрічки
		char msg[] = "Hello";

		// цикл відправки по одному символу
		// процесам з рангом від 1 до 5
		for (int i = 1; i < COUNT_PROCESS; i++){
			MPI_Issend(msg + i - 1, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
			//Sleep(400);
		}
	}
	else {
		// якщо це не 0 процес
		// прийом одного символу 
		// і його вивід на консоль

		// буфер прийому символів
		char buff;
		MPI_Irecv(&buff, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		std::cout << buff;
	}
	MPI_Finalize();
	return 0;
}