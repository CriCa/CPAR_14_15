#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <iostream>

using namespace std;

#define PRIME 0
#define NOTPRIME 1

/// bit operations macros

// bits in long long
#define NUM_BITS 64

// Find array index for Nth bit
#define INDEX_BIT(N) ((N) / NUM_BITS)

// Find position of Nth bit
#define POS_BIT(N) ((N) % NUM_BITS)

// Mark Nth bit
#define MARK_BIT(arr, N) ((arr)[INDEX_BIT(N)] |= (1i64 << POS_BIT(N)))

// Test if Nth bit is prime
#define IS_PRIME_BIT(arr, N) ((arr)[INDEX_BIT(N)] & (1i64 << POS_BIT(N)))

// number of blocks for blocks algorithm
#define N_CHUNKS 4098

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

// number of methods
#define MAX_METHOD 13

// function that returns the method name
string getMethod(int method) {
	switch (method)
	{
	case 0:
		return "Sequential Fast Marking";
	case 1:
		return "Sequential Odd Numbers";
	case 2:
		return "Sequential Reorganized Loops";
	case 3:
		return "Sequential Fast Marking (Bit)";
	case 4:
		return "Sequential Odd Numbers (Bit)";
	case 5:
		return "Sequential Reorganized Loops (Bit)";
	case 6:
		return "Parallel Internal Loop";
	case 7:
		return "Parallel External Loop";
	case 8:
		return "Parallel Odd Numbers";
	case 9:
		return "Parallel Reorganized Loops";
	case 10:
		return "Parallel Internal Loop (Bit)";
	case 11:
		return "Parallel External Loop (Bit)";
	case 12:
		return "Parallel Odd Numbers (Bit)";
	case 13:
		return "Parallel Reorganized Loops (Bit)";
	default:
		return "ERROR";
	}
}

// function that return true if the method uses bitmap or false otherwise
int isBitMethod(int method) {
	return 	method == SB_FM || method == SB_ODD ||
		method == SB_BLOCKS || method == PB_INT ||
		method == PB_EXT || method == PB_ODD ||
		method == PB_BLOCKS;
}

// function that return true if the method uses even numbers or false otherwise
int hasEven(int method) {
	return 	method == S_FM || method == SB_FM ||
		method == P_INT || method == P_EXT ||
		method == PB_INT || method == PB_EXT;
}

// function that counts primes for methods that use bitmap
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

// function that counts primes for normal methods
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