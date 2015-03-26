#include <mpi.h>
#include <iostream>

#define BUFFSIZE 100

// Кількість створюваних процесів
// для корректної роботи - 4
int main(int argc, char **argv)
{
	char msg[BUFFSIZE];
	char msg2[BUFFSIZE];

	void *buff;
	int buff_size;

	int rank;
	int tag = 0;
	MPI_Status status;
	MPI_Request request;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// створення буферу для буферизації повідомлень
	MPI_Buffer_attach(malloc(BUFFSIZE + MPI_BSEND_OVERHEAD), BUFFSIZE + MPI_BSEND_OVERHEAD);

	switch (rank){
	case 0:
		// формування повідомлення для
		// послідовного передавання
		strcpy(msg, "that is the question:");
		std::cout << "from process with rank=" << rank << ", " << msg;

		MPI_Ibsend(&msg, BUFFSIZE, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &request);
		MPI_Buffer_detach(&buff, &buff_size);
		break;
	case 1:
		MPI_Irecv(&msg, BUFFSIZE, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		std::cout << "from process with rank=" << rank << ", " << msg;

		MPI_Ibsend(&msg, BUFFSIZE, MPI_CHAR, 2, tag, MPI_COMM_WORLD, &request);
		MPI_Buffer_detach(&buff, &buff_size);
		break;
	case 2:
		MPI_Irecv(&msg, BUFFSIZE, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		std::cout << "from process with rank=" << rank << ", " << msg;

		MPI_Ibsend(&msg, BUFFSIZE, MPI_CHAR, 3, tag, MPI_COMM_WORLD, &request);
		MPI_Buffer_detach(&buff, &buff_size);
		break;
	case 3:
		MPI_Irecv(&msg, BUFFSIZE, MPI_CHAR, 2, tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);

		std::cout << "from process with rank=" << rank << ", " << msg;
		MPI_Buffer_detach(&buff, &buff_size);
		break;
	}

	MPI_Finalize();
	return 0;
}