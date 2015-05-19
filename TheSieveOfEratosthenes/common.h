#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <iostream>

using namespace std;

// bits in long long
#define NUM_BITS 64

#define CHUNK_SIZE 4098

// Find array index for Nth bit
#define INDEX_BIT(N) (N / NUM_BITS)

// Find position of Nth bit
#define POS_BIT(N) (N % NUM_BITS)

// Set Nth bit
#define MARK_BIT(barr, N) ((barr)[INDEX_BIT(N)] |= (1i64 << POS_BIT(N)))

// Test Nth bit
#define IS_PRIME_BIT(barr, N) ((barr)[INDEX_BIT(N)] & (1i64 << POS_BIT(N)))

#define PRIME 0
#define NOTPRIME 1

/// Methods
#define S_FM 0
#define S_ODD 1
#define S_BLOCKS 2
#define SB_FM 3
#define SB_ODD 4
#define SB_BLOCKS 5
#define P_INT 6
#define P_EXT 7
#define P_ODD 8
#define P_BLOCKS 9
#define PB_INT 10
#define PB_EXT 11
#define PB_ODD 12
#define PB_BLOCKS 13

#define MAX_METHOD 13

void printMethod(int method) {
	switch (method)
	{
	case 0:
		cout << "Sequential Fast Marking";
		break;
	case 1:
		cout << "Sequential Odd Numbers";
		break;
	case 2:
		cout << "Sequential Reorganized Loops";
		break;
	case 3:
		cout << "Sequential Fast Marking (Bit)";
		break;
	case 4:
		cout << "Sequential Odd Numbers (Bit)";
		break;
	case 5:
		cout << "Sequential Reorganized Loops (Bit)";
		break;
	case 6:
		cout << "Parallel Internal Loop";
		break;
	case 7:
		cout << "Parallel External Loop";
		break;
	case 8:
		cout << "Parallel Odd Numbers";
		break;
	case 9:
		cout << "Parallel Reorganized Loops";
		break;
	case 10:
		cout << "Parallel Internal Loop (Bit)";
		break;
	case 11:
		cout << "Parallel External Loop (Bit)";
		break;
	case 12:
		cout << "Parallel Odd Numbers (Bit)";
		break;
	case 13:
		cout << "Parallel Reorganized Loops (Bit)";
		break;
	}
}

int isBitMethod(int method) {
	return 	method == SB_FM || method == SB_ODD ||
		method == SB_BLOCKS || method == PB_INT ||
		method == PB_EXT || method == PB_ODD ||
		method == PB_BLOCKS;
}

int hasEven(int method) {
	return 	method == S_FM || method == SB_FM ||
		method == P_INT || method == P_EXT ||
		method == PB_INT || method == PB_EXT;
}

long long countPrimes(long long * primes, int method, long long limit) {
	long long count = 0;

	if (!hasEven(method)) {
		count = 1;
		for (long long i = 0; 3 + 2 * i < limit; i++)
			if (!IS_PRIME_BIT(primes, i))
				count++;
	}
	else
		for (long long i = 2; i < limit; i++)
			if (!IS_PRIME_BIT(primes, i))
				count++;

	return count;
}

long long countPrimes(unsigned char * primes, int method, long long limit) {
	long long count = 0;

	if (!hasEven(method)) {
		count = 1;
		for (long long i = 0; 3 + 2 * i < limit; i++)
			if (!primes[i])
				count++;
	}
	else {
		for (long long i = 2; i < limit; i++)
			if (!primes[i])
				count++;
	}

	return count;
}

#endif