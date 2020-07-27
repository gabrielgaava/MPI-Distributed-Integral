compile:
	mpicc main.c -o main -lm

slave1:
	mpirun --use-hwthread-cpus -np 2 -quiet ./main
	
slave2:
	mpirun --use-hwthread-cpus -np 3 -quiet ./main
	
slave4:
	mpirun --use-hwthread-cpus -np 5 -quiet ./main
	
slave7:
	mpirun --use-hwthread-cpus -np 8 -quiet ./main

slave10:
	mpirun --use-hwthread-cpus -np 11 -quiet ./main