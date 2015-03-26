#include <mpi.h>
#include <iostream>

// ʳ������ ����������� ������� - 6
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
		// ���������� ������
		char msg[] = "Hello";

		// ���� �������� �� ������ �������
		// �������� � ������ �� 1 �� 5
		for (int i = 1; i < COUNT_PROCESS; i++){
			MPI_Issend(msg + i - 1, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, &request);
			//Sleep(400);
		}
	}
	else {
		// ���� �� �� 0 ������
		// ������ ������ ������� 
		// � ���� ���� �� �������

		// ����� ������� �������
		char buff;
		MPI_Irecv(&buff, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, &status);
		std::cout << buff;
	}
	MPI_Finalize();
	return 0;
}