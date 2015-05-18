#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include "sequential.h"
#include "parallel.h"

void execute() {
	omp_set_dynamic(0);
	omp_set_num_threads(4);

	ulong * primes_bits = NULL; 	// prime, bitmask array
	uchar * primes_char = NULL; 	// prime char array

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
	//sscanf(argv[1], "%d", &exponent);
	//sscanf(argv[2], "%d", &method);

	exponent = 25;
	method = 0;

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
	s_base(primes_char, limit, sqrtLimit);

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

void test() {

}

int main(int argc, char const *argv[])
{
	execute();
	test();

	system("PAUSE");

	return 0;
}
