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
	// setup openmp
	omp_set_dynamic(0);
	omp_set_num_threads(4);

	long long * primesBit = NULL; // prime bitmask array
	unsigned char * primesChar = NULL; // prime char array

	long long limit; // upper bound limit
	long long sqrtLimit; // squareroot of limit
	long long memory; // allocated memory
	long long primeCount; // count of total primes

	double elapseTime;

	// loop methods
	for (int method = 0; method <= MAX_METHOD; method++) {

		// print method
		cout << "Testing method: ";
		printMethod(method);
		cout << endl;

		// loop exponents
		for (int exponent = EXP_LOWER_LIMIT; exponent <= EXP_UPPER_LIMIT; exponent++) {

			// print current exponent
			cout << "Testing n = " << exponent << endl;

			limit = (long long)pow(2.0, exponent);
			sqrtLimit = (long long)sqrt((long double)limit);

			// set memory size
			if (!hasEven(method))
				memory = (long long)round((long double)((limit - 3) / 2) + 1);
			else
				memory = limit;

			// allocate memory
			if (isBitMethod(method)) {
				primesBit = (long long*)malloc(((memory / NUM_BITS) + 1) * sizeof(long long));
				memset(primesBit, PRIME, ((memory / NUM_BITS) + 1));
			}
			else {
				primesChar = (unsigned char*)malloc(memory * sizeof(unsigned char));
				memset(primesChar, PRIME, memory);
			}

			//start clock
			elapseTime = omp_get_wtime();

			// compute primes
			switch (method)
			{
			case 0:
				s_fastMarking(primesChar, limit, sqrtLimit);
				break;
			case 1:
				s_odd(primesChar, limit, sqrtLimit, memory);
				break;
			case 2:
				s_blocks(primesChar, limit, sqrtLimit, memory);
				break;
			case 3:
				s_fastMarking(primesBit, limit, sqrtLimit);
				break;
			case 4:
				s_odd(primesBit, limit, sqrtLimit, memory);
				break;
			case 5:
				s_blocks(primesBit, limit, sqrtLimit, memory);
				break;
			case 6:
				p_internal(primesChar, limit, sqrtLimit);
				break;
			case 7:
				p_external(primesChar, limit, sqrtLimit);
				break;
			case 8:
				p_odd(primesChar, limit, sqrtLimit, memory);
				break;
			case 9:
				p_blocks(primesChar, limit, sqrtLimit, memory);
				break;
			case 10:
				p_internal(primesBit, limit, sqrtLimit);
				break;
			case 11:
				p_external(primesBit, limit, sqrtLimit);
				break;
			case 12:
				p_odd(primesBit, limit, sqrtLimit, memory);
				break;
			case 13:
				p_blocks(primesBit, limit, sqrtLimit, memory);
				break;
			}

			//stop clock
			elapseTime = omp_get_wtime() - elapseTime;

			// count primes
			if (isBitMethod(method))
				primeCount = countPrimes(primesBit, method, limit);
			else
				primeCount = countPrimes(primesChar, method, limit);


			//print results
			cout << "Primes found: " << primeCount << endl
				<< "Total time: " << elapseTime << " secs" << endl << endl;
			
			// free memory
			if (isBitMethod(method))
				free(primesBit);
			else
				free(primesChar);
		}

		// spacing for next method
		cout << endl;
	}

	system("PAUSE");

	return 0;
}