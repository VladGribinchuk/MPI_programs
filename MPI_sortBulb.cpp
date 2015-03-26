// СОРТУВАННЯ БУЛЬБАШКОЮ
//		алгоритм паралельної схеми
// 
///	1) Розділення масиву між процесами
//	ПРОЦ0		ПРОЦ1		ПРОЦ2		ПРОЦ3
//	2 5			8 7			7 1			9 3
//
///	2) Внутрішнє сортування
//	ПРОЦ0		ПРОЦ1		ПРОЦ2		ПРОЦ3
//	2 5			7 8			1 7			3 9
//
///	3) Парні ПРОЦ: взаємний обмін
//					+ внутрішнє сортування  
//					+ розбиття (парні - з молодшою част.
//								непарні - зі старшою)
//	ПРОЦ0		ПРОЦ1		ПРОЦ2		ПРОЦ3
//	2 5 7 8		7 8	2 5		1 7 3 9		3 9 1 7
//	ПРОЦ0		ПРОЦ1		ПРОЦ2		ПРОЦ3
//	2 5 | 7 8	2 5 | 7 8	1 3 | 7 9	1 3 | 7 9
//	ПРОЦ0		ПРОЦ1		ПРОЦ2		ПРОЦ3
//	2 5 		7 8			1 3			7 9
//
///	4) Непарні ПРОЦ: аналогічно до п.3
//
/// Пункт 3-4 в ЦИКЛ на [кількість процесів] ітерацій
#include <mpi.h>
#include <iostream>
#define COUNT_ELEMENT_DEFAULT 12
void bulb_sort(int* input_arr, int size)
{
	int i, j, tmp;
	for (i = 0; i < size - 1; i++)
		for (j = size - 1; j > i; j--)
			if (input_arr[j] < input_arr[j - 1]){
				tmp = input_arr[j];
				input_arr[j] = input_arr[j - 1];
				input_arr[j - 1] = tmp;
			}
}

int main(int argc, char **argv)
{
	int rank;
	int tag = 0;
	int root = 0;
	int count_process,
		capacity_arr = COUNT_ELEMENT_DEFAULT,
		count_element;
	MPI_Status status;
	MPI_Request request;

	// початковий невідсорт. масив
	int *primary_data_arr;
	// кінцевий відсортований масив
	int *result_data_arr;
	// допоміжні буфери
	int *temp_data_arr;
	int *merge_data_arr;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	primary_data_arr = new int[capacity_arr];
	result_data_arr = new int[capacity_arr];
	if (rank == 0){
		//
		// Заповнення початкового масиву
		srand(MPI_Wtime());
		for (int i = 0; i < capacity_arr; i++)
			primary_data_arr[i] = rand() % 100;
		//
		// Розмір для розбиття масиву
		count_element = capacity_arr / count_process;
	} 

	// Посилка розміру розбиття від root'a
	MPI_Bcast(&count_element, 1, MPI_INT, root, MPI_COMM_WORLD);
	// Буферний масив для вмісту отриманої частини поч. масиву
	temp_data_arr = new int[count_element];
	// Роздача початкового масиву по частинам ємністю count_element
	MPI_Scatter(primary_data_arr, count_element, MPI_INT, temp_data_arr, count_element, MPI_INT, root, MPI_COMM_WORLD);
	// Початкове внутрішнє сортування для кожної частинки
	bulb_sort(temp_data_arr, count_element);
	// Допоміжний буфер для злиття при обміні
	merge_data_arr = new int[count_element * 2];

	// Цикл з парними/непарними ітераціями
	for (int i = 0; i < count_process; i++){
		if (i % 2 == 0){
			//
			// Ітерація парна, в обміні беруть участь
			// (0-1)(2-3)(4-5)...
			if (rank%2 == 0){
				if (rank < (count_process - 1)){
					//
					// Якщо процес парний і якщо він не останній
					// Виконуємо взаємний обмін з наступним -
					//	результат в merge_data_arr
					for (int k = 0; k < count_element; k++)
						merge_data_arr[k] = temp_data_arr[k];
					MPI_Ssend(temp_data_arr, count_element, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
					MPI_Recv(merge_data_arr + count_element, count_element, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
					bulb_sort(merge_data_arr, count_element * 2);
					for (int k = 0; k < count_element; k++)
						temp_data_arr[k] = merge_data_arr[k];
				}
			} 
			else {
				//
				// Правий процес відносно парного
				for (int k = 0; k < count_element; k++)
					merge_data_arr[k] = temp_data_arr[k];
				MPI_Recv(merge_data_arr + count_element, count_element, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
				MPI_Ssend(temp_data_arr, count_element, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
				bulb_sort(merge_data_arr, count_element * 2);
				for (int k = 0; k < count_element; k++)
					temp_data_arr[k] = merge_data_arr[k + count_element];
			}
		} 
		else {
			//
			// Ітерація НЕпарна, в обміні беруть участь
			// (1-2)(3-4)(5-6)...
			if (rank%2 == 1){
				if (rank < (count_process - 1)){
					//
					// Якщо процес НЕпарний і якщо він не останній
					// Виконуємо взаємний обмін з наступним -
					//	результат в merge_data_arr
					for (int k = 0; k < count_element; k++)
						merge_data_arr[k] = temp_data_arr[k];
					MPI_Ssend(temp_data_arr, count_element, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
					MPI_Recv(merge_data_arr + count_element, count_element, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
					bulb_sort(merge_data_arr, count_element * 2);
					for (int k = 0; k < count_element; k++)
						temp_data_arr[k] = merge_data_arr[k];
				}
			}
			else {
				//
				// Правий процес відносно НЕпарного
				// 0 участі не приймає!!
				if (rank != 0){
					for (int k = 0; k < count_element; k++)
						merge_data_arr[k] = temp_data_arr[k];
					MPI_Recv(merge_data_arr + count_element, count_element, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
					MPI_Ssend(temp_data_arr, count_element, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
					bulb_sort(merge_data_arr, count_element * 2);
					for (int k = 0; k < count_element; k++)
						temp_data_arr[k] = merge_data_arr[k + count_element];
				}
			}
		}// end if (i%2==0) else ...
	}// end for()
	
	//
	// Збираємо по частинкам результуючий масив
	MPI_Gather(temp_data_arr, count_element, MPI_INT, result_data_arr, count_element, MPI_INT, root, MPI_COMM_WORLD);
	// і виводимо його на екран
	if (rank == 0)
		for (int i = 0; i < capacity_arr; i++)
			std::cout << result_data_arr[i] << "  ";
	// Очистка виділеної пам'яті
	delete[] primary_data_arr;
	delete[] temp_data_arr;
	delete[] result_data_arr;
	delete[] merge_data_arr;
	MPI_Finalize();
	return 0;
}