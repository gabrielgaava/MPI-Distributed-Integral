compile:
	mpicc main.c -o main -lm

slave2:
	mpirun -np 2 -quiet ./main
	
slave3:
	mpirun -np 3 -quiet ./main
	
slave5:
	mpirun -np 5 -quiet ./main
	
slave10:
	mpirun -np 10 -quiet ./main
	
