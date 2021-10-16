test:
	mpicc wordcount_test.c -o wordcount_test
	
	mpirun -np 1 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 2 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 3 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 4 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 5 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 6 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 7 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv
	mpirun -np 8 --hostfile hfile ./wordcount_test /home/pcpc/files /home/pcpc/results.csv /home/pcpc/utils/strong_results.csv

	mpirun -np 1 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np1 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 2 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np2 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 3 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np3 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 4 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np4 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 5 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np5 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 6 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np6 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 7 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np7 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv
	mpirun -np 8 --hostfile hfile ./wordcount_test /home/pcpc/files_weak/np8 /home/pcpc/results.csv /home/pcpc/utils/weak_results.csv


