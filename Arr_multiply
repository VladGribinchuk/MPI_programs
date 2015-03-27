#include <cstdlib>
#include <iostream>
#include <ctime>

#define SIZE 4

void main()
{
	// allocate memory for array
	int** a = new int* [SIZE];
	for(int i=0;i<SIZE;++i)
		a[i] = new int[SIZE];
	int* b = new int[SIZE];
	int* res = new int[SIZE];

	srand(time(NULL));
	for(int i=0;i<SIZE;b[i++] = rand()%100){
		res[i] = 0;
		for(int j=0;j<SIZE;a[i][j++] = rand()%100);
	}

	std::cout << "a\n";
	for(int i=0;i<SIZE;++i){
		for(int j=0;j<SIZE;++j)
			std::cout << a[i][j] << "\t";
		std::cout << std::endl;
	}
	std::cout << "\nb\n";
	for(int i=0;i<SIZE;++i)
		std::cout << b[i] << "\n";
	
	for(int i=0;i<SIZE;++i)
		for(int j=0;j<SIZE;++j)
			res[i] += a[i][j] * b[j];

	std::cout << "\nres = a * b\n";
	for(int i=0;i<SIZE;++i)
		std::cout << res[i] << "\n";

	// free memory
	for(int i=0;i<SIZE;++i)
		delete []a[i];
	delete []a;
	delete []b;
	delete []res;

	system("pause");
}
