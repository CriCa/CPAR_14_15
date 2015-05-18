#ifndef TYPES
#define TYPES

#include <stdio.h>

// data types
typedef unsigned long long llong;
typedef long long ulong;
typedef unsigned char uchar;

// bits in ulong
#define NUM_BITS 32

#define SEGMENT_SIZE 220000
#define TEMP_SIZE 320000

// bit operations
#define IND_BITS(N) ((N) / NUM_BITS)
/* Find array index for Nth bit */

#define POS_BITS(N) ((N) % NUM_BITS)
/* Find position of Nth bit */

#define SET_BIT(barr, N) ((barr)[IND_BITS(N)] |= (1 << POS_BITS(N)))
/* Set Nth bit */

#define TST_BIT(barr, N) ((barr)[IND_BITS(N)] & (1 << POS_BITS(N)))
/* Test Nth bit */

#define PRIME 0
#define COMPOSITE 1

/// Methods
#define SC_BASIC 		0
#define SC_BASICBITS 	1
#define SC_OPT 			2
#define SC_OPTBITS 		3
#define MC_OMP 			4
#define MC_OMPBITS 		5
#define MC_OMP2 		6
#define MC_OMP2BITS 	7
#define MC_OMPOPT 		8
#define MC_OMPOPTBITS	9

#define SC_CACHED		10
#define SC_CACHEDBITS	11
#define MC_CACHEDBITS	12


int hasBitMask(int m) {
	return 	m == SC_BASICBITS || m == SC_OPTBITS ||
		m == MC_OMPBITS || m == MC_OMP2BITS ||
		m == MC_OMPOPTBITS || m == SC_CACHEDBITS ||
		m == MC_CACHEDBITS;
}

int hasEven(int m) {
	return 	m == SC_BASIC || m == SC_BASICBITS ||
		m == MC_OMP || m == MC_OMP2 ||
		m == MC_OMPBITS || m == MC_OMP2BITS;
}

ulong countPrimesBits(ulong * primes, int method, ulong limit) {
	ulong k, count = 0;

	if (!hasEven(method)) {
		count = 1;
		for (k = 0; 3 + 2 * k < limit; k++) {
			if (!TST_BIT(primes, k)) {
				count++;
				//printf("0");
			} //else
			//printf("1");
		}
		//printf("\n");
	}
	else {
		for (k = 2; k < limit; k++) {
			if (!TST_BIT(primes, k)) {
				count++;
			}
		}
	}

	return count;
}

ulong countPrimesChar(uchar * primes, int method, ulong limit) {
	ulong k, count = 0;

	if (!hasEven(method)) {
		count = 1;
		for (k = 0; 3 + 2 * k < limit; k++) {
			if (!primes[k])
				count++;
			//printf("%d", primes[k]);
		}
		//printf("\n");
	}
	else {
		for (k = 2; k < limit; k++) {
			if (!primes[k]) {
				count++;
			}
		}
	}

	return count;
}

void printMethod(int method) {
	switch (method) {
	case SC_BASIC:
		printf("Single Core Fast Marking");
		break;
	case SC_BASICBITS:
		printf("Single Core Fast Marking BitMask");
		break;
	case SC_OPT:
		printf("Single Core Optimization");
		break;
	case SC_OPTBITS:
		printf("Single Core Optimization BitMask");
		break;
	case MC_OMP:
		printf("OMP Fast Marking");
		break;
	case MC_OMPBITS:
		printf("OMP Fast Marking BitMask");
		break;
	case MC_OMP2:
		printf("OMP2 Fast Marking");
		break;
	case MC_OMP2BITS:
		printf("OMP2 Fast Marking BitMask");
		break;
	case MC_OMPOPT:
		printf("OMP Optimization");
		break;
	case MC_OMPOPTBITS:
		printf("OMP Optimization BitMask");
		break;
	case SC_CACHED:
		printf("Single Core Blocks");
		break;
	case SC_CACHEDBITS:
		printf("Single Core Blocks BitMask");
		break;
	}
	printf("\n");
}


#endif
