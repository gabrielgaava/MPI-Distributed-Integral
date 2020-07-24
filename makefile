compile:
	mpicc main.c -o main -lm

slave1:
	mpirun -np 2 -quiet ./main
	
slave2:
	mpirun -np 3 -quiet ./main
	
slave4:
	mpirun -np 5 -quiet ./main
	
slave10:
	mpirun -np 11 -quiet ./main
	
