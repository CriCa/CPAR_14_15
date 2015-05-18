#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include "sequencial.h"
#include "parallel.h"

int main(int argc, char const *argv[])
{
	omp_set_dynamic(0);
	omp_set_num_threads(4);

	ulong * primes_bits; 	// prime, bitmask array
	uchar * primes_char; 	// prime char array

	ulong
		limit, 				// upper bound limit
		sqrtLimit, 			// squareroot of limit
		space, 				// allocated space
		totalPrimes; 		// count of total primes

	int
		exponent, 			// 2^n exponent
		method; 			// method chosen

	double elapseTime;

	// parse arguments
	sscanf(argv[1], "%d", &exponent);
	sscanf(argv[2], "%d", &method);

	limit = pow((float)2, exponent);
	sqrtLimit = sqrt((long double)limit);

	// set memory size
	if (!hasEven(method))
		space = round((long double)((limit - 3) / 2) + 1);
	else
		space = limit;

	// allocate memory
	if (hasBitMask(method)) {
		primes_bits = (ulong*)malloc(((space / NUM_BITS) + 1) * sizeof(ulong));
		memset(primes_bits, PRIME, ((space / NUM_BITS) + 1));
	}
	else {
		primes_char = (uchar*)malloc(space * sizeof(uchar));
		memset(primes_char, PRIME, space);
	}

	//start clock
	elapseTime = omp_get_wtime();

	// compute primes
	switch (method) {
	case SC_BASIC:
		SC_Basic(primes_char, limit, sqrtLimit);
		break;
	case SC_BASICBITS:
		SC_BasicBits(primes_bits, limit, sqrtLimit);
		break;
	case SC_OPT:
		SC_Opt(primes_char, limit, sqrtLimit, space);
		break;
	case SC_OPTBITS:
		SC_OptBits(primes_bits, limit, sqrtLimit, space);
		break;
	case MC_OMP:
		openMP(primes_char, limit, sqrtLimit);
		break;
	case MC_OMPBITS:
		openMPBits(primes_bits, limit, sqrtLimit);
		break;
	case MC_OMP2:
		openMP2(primes_char, limit, sqrtLimit);
		break;
	case MC_OMP2BITS:
		openMP2Bits(primes_bits, limit, sqrtLimit);
		break;
	case MC_OMPOPT:
		openMPOpt(primes_char, limit, sqrtLimit, space);
		break;
	case MC_OMPOPTBITS:
		openMPOptBits(primes_bits, limit, sqrtLimit, space);
		break;
	case SC_CACHED:
		SC_Cached(primes_char, limit, sqrtLimit, space);
		break;
	case SC_CACHEDBITS:
		SC_CachedBits(primes_bits, limit, sqrtLimit, space);
		break;
	case MC_CACHEDBITS:
		openMPOptCacheBits(primes_bits, limit, sqrtLimit, space);
		break;
	}

	//stop clock
	elapseTime = omp_get_wtime() - elapseTime;

	// count primes
	if (hasBitMask(method))
		totalPrimes = countPrimesBits(primes_bits, method, limit);
	else
		totalPrimes = countPrimesChar(primes_char, method, limit);


	//print results
	printf("#################################\n");
	printf("Exp: %d\n", exponent);
	printMethod(method);
	printf("Primes found: %lu\n", totalPrimes);
	printf("Total time: %g\n", elapseTime);
	printf("#################################\n");


	// free memory
	if (hasBitMask(method)) {
		free(primes_bits);
	}
	else {
		free(primes_char);
	}

	return 0;
}
