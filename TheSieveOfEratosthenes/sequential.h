#ifndef SEQUENTIAL
#define SEQUENTIAL

#include "common.h"

inline void s_fastMarking(unsigned char* primes, long long limit, long long sqrtLimit) {
	register long long k, mark;

	for (k = 2; k <= sqrtLimit; k++)
		if (primes[k] == PRIME)
			for (mark = k * k; mark <= limit; mark += k)
				primes[mark] = NOTPRIME;
}

inline void s_fastMarking(long long* primes, long long limit, long long sqrtLimit) {
	register long long k, mark;

	for (k = 2; k <= sqrtLimit; k++)
		if (!IS_PRIME_BIT(primes, k))
			for (mark = k * k; mark <= limit; mark += k)
				MARK_BIT(primes, mark);
}

inline void s_odd(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (primes[i] == PRIME) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
			mark = (k * k - 3) / 2; // (limite maximo - 3) / 2

			for (; mark <= size; mark += k)
				primes[mark] = NOTPRIME;
		}
	}
}

inline void s_odd(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (!IS_PRIME_BIT(primes, i)) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
			mark = (k * k - 3) / 2; // (limite maximo - 3) / 2

			for (; mark <= size; mark += k)
				MARK_BIT(primes, mark);
		}
	}
}

inline void s_blocks(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long ind, k, mark, i;
	long long chunks = 4098;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh, j;

	//proccess own block
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (size) / (chunks + 1);
		chunkHighIndex = (i + 1) * (size) / (chunks + 1) - 1;
		chunkLow = chunkLowIndex + chunkLowIndex + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + 3;

		ind = 0;

		do {
			k = ind + ind + 3;
			mark = k * k;

			if (mark > chunkLow)
				mark = ((mark - 3) >> 1) - ((chunkLow - 3) >> 1);
			else {
				mark = chunkLow % k;
				if (mark != PRIME) {
					if (k > chunkLow % (k + k)) mark = (k - mark) >> 1;
					if (k < chunkLow % (k + k)) mark = k - (mark >> 1);
				}
			}

			mark += chunkLowIndex;

			for (j = mark; j <= chunkHighIndex; j += k)
				primes[j] = NOTPRIME;

			ind++;
			while (primes[ind] == NOTPRIME)
				ind++;

		} while (k * k <= chunkHigh);
	}
}

inline void s_blocks(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long ind, k, mark, i;
	long long chunks = 4098;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh, j;

	//proccess own block
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (size) / (chunks + 1);
		chunkHighIndex = (i + 1) * (size) / (chunks + 1) - 1;
		chunkLow = chunkLowIndex + chunkLowIndex + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + 3;

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
				MARK_BIT(primes, j);

			ind++;
			while (IS_PRIME_BIT(primes, ind))
				ind++;

		} while (k * k <= chunkHigh);
	}
}

#endif