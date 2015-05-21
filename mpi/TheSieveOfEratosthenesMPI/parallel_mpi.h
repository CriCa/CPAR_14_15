#ifndef PARALLEL_MPI
#define PARALLEL_MPI

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <mpi.h>

// macros

#define BLOCK_LOW(i,p,n) 2 + ((i)*(n-1)/(p))
#define BLOCK_HIGH(i,p,n) (BLOCK_LOW((i)+1,p,n) - 1)

#define PRIME 0
#define NOTPRIME 1

#define N_CHUNCKS 4098;

#define MASTER 0

using namespace std;

long long countPrimes(unsigned char* numbers, long long blockSize) {
	long long i, count = 0;

	for (i = 0; i < blockSize; i++)
		if (numbers[i] == PRIME)
			count++;

	return count;
}

long long countPrimes_seed(unsigned char* numbers, long long blockHigh) {
	long long i, count = 0;

	for (i = 0; i <= blockHigh; i++)
		if (numbers[i] == PRIME)
			count++;

	return count;
}

long long countPrimes_seed(unsigned char* numbers, long long blockLow, long long blockHigh) {
	long long i, count = 0;

	for (i = blockLow; i <= blockHigh; i++)
		if (numbers[i - blockLow] == PRIME)
			count++;

	return count;
}

double mpi_basic(int rank, int size, long long limit, long long sqrtLimit) {
	unsigned char *primes;
	long long k, mark, blockLow, blockHigh, blockSize, index;
	double maxTime;
	
	// compute block limits
	blockLow = BLOCK_LOW(rank, size, limit);
	blockHigh = BLOCK_HIGH(rank, size, limit);
	blockSize = blockHigh - blockLow;

	// allocate memory space
	primes = (unsigned char*)calloc(blockSize, sizeof(unsigned char));

	k = 2;
	if (rank == MASTER) index = 0;

	// start clock
	double start_time = omp_get_wtime();

	// compute primes
	do {
		mark = k * k;

		if (mark > blockLow)
			mark = mark - blockLow;
		else {
			mark = blockLow % k;
			if (mark != 0)
				mark = k - mark;
		}

		for (; mark < blockSize; mark += k)
			primes[mark] = 1;

		// only master
		if (rank == MASTER) {
			while (primes[++index]);
			k = index + 2;
		}

		MPI_Bcast(&k, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
	} while (k <= sqrtLimit);

	// stop clock
	double time = omp_get_wtime() - start_time;

	// compute results
	if (rank == MASTER) {
		maxTime = time;
		long long count = 0, servantCount;
		int servant;

		count += countPrimes(primes, blockSize);

		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&servantCount, 1, MPI_LONG_LONG, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += servantCount;
		}

		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&time, 1, MPI_DOUBLE, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maxTime = fmax(maxTime, time);
		}

		// print results
		cout << "Primes Found: " << count << endl
			<< "Exec time: " << maxTime << " secs" << endl;
	}
	else {
		long long count = countPrimes(primes, blockSize);
		MPI_Send(&count, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

		MPI_Send(&time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	
	free(primes);

	return maxTime;
}

double mpi_seed(int rank, int size, long long limit, long long sqrtLimit) {
	unsigned char *primes, *seeds;
	long long nOdd, k, mark, blockLow, blockHigh, blockSize, ind, realMin;
	double maxTime;

	nOdd = ((limit - 3) / 2) + 1;
	
	blockLow = BLOCK_LOW(rank, size, nOdd);
	blockHigh = BLOCK_HIGH(rank, size, nOdd);
	blockSize = blockHigh - blockLow + 1;

	primes = (unsigned char*)calloc(blockSize, sizeof(unsigned char));
	seeds = (unsigned char*) calloc(sqrtLimit + 2, sizeof(unsigned char));

	realMin = blockLow + blockLow + 3;

	// start clock
	double start_time = omp_get_wtime();

	// pre-proccess seeds
	for (ind = 0; ind + ind + 3 <= sqrtLimit; ind++)
		if (seeds[ind] == 0) {
			k = ind + ind + 3;
			mark = (k * k - 3) / 2;

			for (; mark <= sqrtLimit; mark += k)
				seeds[mark] = 1;
		}

	// compute primes
	for (ind = 0; ind + ind + 3 <= sqrtLimit;) {
		k = ind + ind + 3;
		mark = k * k;

		if (mark > realMin)
			mark = ((mark - 3) >> 1) - blockLow;
		else {
			mark = realMin % k;
			if (mark != 0) {
				if (mark % 2 != 0) {
					mark = ((((k - mark) + realMin) - 3) >> 1) - blockLow;
				}
				else {
					mark = (((((k - mark) + realMin) + k) - 3) >> 1) - blockLow;
				}
			}
		}

		for (; mark < blockSize; mark += k)
			primes[mark] = 1;

		while (seeds[++ind]);
	}

	// stop clock
	double time = omp_get_wtime() - start_time;

	if (rank == MASTER) {
		maxTime = time;
		long long count = 1, servantCount;
		int servant;

		count += countPrimes_seed(primes, blockHigh);
		
		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&servantCount, 1, MPI_LONG_LONG, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += servantCount;
		}

		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&time, 1, MPI_DOUBLE, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maxTime = fmax(maxTime, time);
		}

		// print results
		cout << "Primes Found: " << count << endl
			<< "Exec time: " << maxTime << " secs" << endl;
	}
	else {
		long long count = countPrimes_seed(primes, blockLow, blockHigh);
		MPI_Send(&count, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

		MPI_Send(&time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	free(primes);

	return maxTime;
}

double mpi_blocks(int rank, int size, long long limit, long long sqrtLimit) {
	unsigned char *primes, *seeds;
	long long nOdd, i, k, mark, blockLow, blockHigh, blockSize, ind, realMin;
	double maxTime;

	nOdd = ((limit - 3) / 2) + 1;

	blockLow = BLOCK_LOW(rank, size, nOdd);
	blockHigh = BLOCK_HIGH(rank, size, nOdd);
	blockSize = blockHigh - blockLow + 1;

	primes = (unsigned char*)calloc(blockSize, sizeof(unsigned char));
	seeds = (unsigned char*)calloc(sqrtLimit + 2, sizeof(unsigned char));

	realMin = blockLow + blockLow + 3;

	long long chunks = N_CHUNCKS;
	long long chunkLowIndex, chunkHighIndex, chunkSize, chunkLow, chunkHigh, j;

	// start clock
	double start_time = omp_get_wtime();

	// pre-proccess seeds
	for (ind = 0; ind + ind + 3 <= sqrtLimit; ind++)
		if (seeds[ind] == 0) {
			k = ind + ind + 3;
			mark = (k * k - 3) / 2;

			for (; mark <= sqrtLimit; mark += k)
				seeds[mark] = 1;
		}

	// compute primes
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (blockSize) / (chunks + 1);
		chunkHighIndex = (i + 1) * (blockSize) / (chunks + 1) - 1;
		chunkSize = (chunkHighIndex - chunkLowIndex) + 1;
		chunkLow = chunkLowIndex + chunkLowIndex + blockLow + blockLow + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + blockLow + blockLow + 3;

		ind = 0;

		do {
			k = ind + ind + 3;
			mark = k * k;


			if (mark > chunkLow)
				mark = ((mark - 3) >> 1) - ((chunkLow - 3) >> 1);
			else {
				mark = chunkLow % k;
				if (mark != 0) {
					if (k > chunkLow % (k + k)) mark = (k - mark) >> 1;
					if (k < chunkLow % (k + k)) mark = k - (mark >> 1);
				}
			}

			mark += chunkLowIndex;

			for (j = mark; j <= chunkHighIndex; j += k)
				primes[j] = 1;

			while (seeds[++ind]);
		} while (k * k <= chunkHigh);
	}

	// stop clock
	double time = omp_get_wtime() - start_time;

	if (rank == MASTER) {
		maxTime = time;
		long long count = 1, servantCount;
		int servant;

		count += countPrimes_seed(primes, blockHigh);

		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&servantCount, 1, MPI_LONG_LONG, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += servantCount;
		}

		for (rank = 1; rank < size; rank++) {
			MPI_Recv(&time, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maxTime = fmax(maxTime, time);
		}

		// print results
		cout << "Primes Found: " << count << endl
			<< "Exec time: " << maxTime << " secs" << endl;
	}
	else {
		long long count = countPrimes_seed(primes, blockLow, blockHigh);
		MPI_Send(&count, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

		MPI_Send(&time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	free(primes);

	return maxTime;
}

double mpi_blocks_p(int rank, int size, long long limit, long long sqrtLimit) {
	unsigned char *primes, *seeds;
	long long nOdd, i, k, mark, blockLow, blockHigh, blockSize, ind, realMin;
	double maxTime;

	nOdd = ((limit - 3) / 2) + 1;

	blockLow = BLOCK_LOW(rank, size, nOdd);
	blockHigh = BLOCK_HIGH(rank, size, nOdd);
	blockSize = blockHigh - blockLow + 1;

	primes = (unsigned char*)calloc(blockSize, sizeof(unsigned char));
	seeds = (unsigned char*)calloc(sqrtLimit + 2, sizeof(unsigned char));

	realMin = blockLow + blockLow + 3;

	long long chunks = N_CHUNCKS;
	long long chunkLowIndex, chunkHighIndex, chunkSize, chunkLow, chunkHigh, j;

	// start clock
	double start_time = omp_get_wtime();

	// pre-proccess seeds
	for (ind = 0; ind + ind + 3 <= sqrtLimit; ind++)
		if (seeds[ind] == 0) {
			k = ind + ind + 3;
			mark = (k * k - 3) / 2;

			for (; mark <= sqrtLimit; mark += k)
				seeds[mark] = 1;
		}

	// compute primes
	#pragma omp parallel for
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (blockSize) / (chunks + 1);
		chunkHighIndex = (i + 1) * (blockSize) / (chunks + 1) - 1;
		chunkSize = (chunkHighIndex - chunkLowIndex) + 1;
		chunkLow = chunkLowIndex + chunkLowIndex + blockLow + blockLow + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + blockLow + blockLow + 3;

		ind = 0;

		do {
			k = ind + ind + 3;
			mark = k * k;


			if (mark > chunkLow)
				mark = ((mark - 3) >> 1) - ((chunkLow - 3) >> 1);
			else {
				mark = chunkLow % k;
				if (mark != 0) {
					if (k > chunkLow % (k + k)) mark = (k - mark) >> 1;
					if (k < chunkLow % (k + k)) mark = k - (mark >> 1);
				}
			}

			mark += chunkLowIndex;

			for (j = mark; j <= chunkHighIndex; j += k)
				primes[j] = 1;

			while (seeds[++ind]);
		} while (k * k <= chunkHigh);
	}

	// stop clock
	double time = omp_get_wtime() - start_time;

	if (rank == MASTER) {
		maxTime = time;
		long long count = 1, servantCount;
		int servant;

		count += countPrimes_seed(primes, blockHigh);

		for (servant = 1; servant < size; servant++) {
			MPI_Recv(&servantCount, 1, MPI_LONG_LONG, servant, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += servantCount;
		}

		for (rank = 1; rank < size; rank++) {
			MPI_Recv(&time, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			maxTime = fmax(maxTime, time);
		}

		// print results
		cout << "Primes Found: " << count << endl
			<< "Exec time: " << maxTime << " secs" << endl;
	}
	else {
		long long count = countPrimes_seed(primes, blockLow, blockHigh);
		MPI_Send(&count, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

		MPI_Send(&time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	free(primes);

	return maxTime;
}

#endif