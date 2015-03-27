MPI_Init(&argc, &argv);

	int rank, size;
	int root = 0;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	int** a;
	int* b;
	int* bufa;
	int* bufb;
	int* res;
	int* res2;
	int* result;

	int tmp;
	int send;
	int sendb;
	int sendres;

	a = new int* [SIZE];
		for(int i=0;i<SIZE;++i)
			a[i] = new int[SIZE];
		
	b = new int[SIZE];

	if(rank == 0){
		srand(MPI_Wtime());
		for(int i=0;i<SIZE;b[i++] = rand()%4){
			for(int j=0;j<SIZE;a[i][j++] = rand()%4);
		}

		
		std::cout << "\na\n";
		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++)
				std::cout << a[i][j] << " " ;
			std::cout << "\n";
		}
		std::cout << "\nb\n";
		for(int i=0;i<SIZE;i++)
			std::cout << b[i] << " ";
	}
		tmp = SIZE;
		send = tmp*tmp/size;
		MPI_Bcast(&send,1,MPI_INT,root,MPI_COMM_WORLD);
		bufa = new int[send];
		MPI_Scatter(a,send,MPI_INT,bufa,send,MPI_INT,root,MPI_COMM_WORLD);
		//if(send>4){
		//	sendb = 4;
		//	MPI_Bcast(&sendb,1,MPI_INT,root,MPI_COMM_WORLD);
		//	bufb = new int[sendb];
		//	MPI_Bcast(b,sendb,MPI_INT,root,MPI_COMM_WORLD);
		//}
		//else{
			sendb = send;
			MPI_Bcast(&sendb,1,MPI_INT,root,MPI_COMM_WORLD);
			bufb = new int[sendb];
			MPI_Scatter(b,sendb,MPI_INT,bufb,sendb,MPI_INT,root,MPI_COMM_WORLD);
		//}

		

	//}
	//else {
	//		MPI_Bcast(&send,1,MPI_INT,root,MPI_COMM_WORLD);
	//		bufa = new int[send];
	//		MPI_Scatter(a,send,MPI_INT,bufa,send,MPI_INT,root,MPI_COMM_WORLD);

			//if(send>4){
			//	MPI_Bcast(&sendb,1,MPI_INT,root,MPI_COMM_WORLD);
			//	bufb = new int[sendb];
			//	b = new int[sendb];
			//	MPI_Bcast(b,sendb,MPI_INT,root,MPI_COMM_WORLD);
			//	for(int i=0;i<send;++i)
			//		bufb[i] = b[i];
			//}
			//else{
	//			MPI_Bcast(&sendb,1,MPI_INT,root,MPI_COMM_WORLD);
	//			bufb = new int[sendb];
	//			MPI_Scatter(b,sendb,MPI_INT,bufb,sendb,MPI_INT,root,MPI_COMM_WORLD);
			//}

			
	//}

	//if (send <=4){
	//			res = new int[sendb];
				//for(int i=0; i<sendb; ++i) res[i] = 0;
	//			for(int i=0; i<sendb; ++i){
	//				res[i] = bufa[i]*bufb[i];
	//			}
	//		} 
	//		else {
				res = new int[sendb];
				for(int i=0; i<sendb; ++i){
					res[i] = bufa[i]*bufb[i];
				}
	//		}
			sendres = sendb;

	//if(rank == 0)
		res2 = new int[sendres*size];
		
	MPI_Gather(res,sendres,MPI_INT,res2,sendres,MPI_INT,root,MPI_COMM_WORLD);

	result = new int[SIZE];

	if(rank == 0){
		
		for(int i=0;i<SIZE;++i)
			result[i] = 0;
		int k = 0;
		for(int i=0;i<SIZE;++i){
			for(int j=0;j<(4/sendres);++j){
				result[i] += res2[k];
				k=k+1;
			}
		}

		std::cout << "result:\n";
		for(int i=0; i<SIZE; ++i){
			std::cout << result[i] << "\n";
		}

	}

	for(int i=0;i<SIZE;++i)
		delete []a[i];
	delete []a;
	delete []b;
	delete []bufa;
	delete []bufb;
	delete []res;
	delete []res2;
	delete []result;

	MPI_Finalize();
	//system("pause");
}
