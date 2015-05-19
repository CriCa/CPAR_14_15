#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include "sequential.h"
#include "parallel.h"

// defines
#define EXP_LOWER_LIMIT 25
#define EXP_UPPER_LIMIT 32

int main(int argc, char const *argv[])
{
	omp_set_dynamic(0);
	omp_set_num_threads(4);

	llong * primes_bits = NULL; 	// prime, bitmask array
	uchar * primes_char = NULL; 	// prime char array

	llong
		limit, 				// upper bound limit
		sqrtLimit, 			// squareroot of limit
		space, 				// allocated space
		totalPrimes; 		// count of total primes

	int
		exponent, 			// 2^n exponent
		method; 			// method chosen

	double elapseTime;

	// parse arguments
	//sscanf(argv[1], "%d", &exponent);
	//sscanf(argv[2], "%d", &method);

	for (exponent = EXP_UPPER_LIMIT; exponent <= EXP_UPPER_LIMIT; exponent++) {
		method = 0;

		limit = (llong)pow((float)2, exponent);
		sqrtLimit = (llong)sqrt((long double)limit);

		// set memory size
		if (!hasEven(method))
			space = (llong)round((long double)((limit - 3) / 2) + 1);
		else
			space = limit;

		// allocate memory
		if (hasBitMask(method)) {
			primes_bits = (llong*)malloc(((space / NUM_BITS) + 1) * sizeof(llong));
			memset(primes_bits, PRIME, ((space / NUM_BITS) + 1));
		}
		else {
			primes_char = (uchar*)malloc(space * sizeof(uchar));
			memset(primes_char, PRIME, space);
		}

		//start clock
		elapseTime = omp_get_wtime();

		// compute primes
		s_fastMarking(primes_char, limit, sqrtLimit);

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
	}

	system("PAUSE");

	return 0;
}
