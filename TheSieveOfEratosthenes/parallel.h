#ifndef PARALLEL
#define PARALLEL

#include <omp.h>
#include "common.h"

// parallel internal loop
inline void p_internal(unsigned char* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	for (i = 2; i <= sqrtLimit; i++)
		if (primes[i] == PRIME) {
			#pragma omp parallel for
			for (j = i * i; j <= limit; j += i)
				primes[j] = NOTPRIME;
		}
}

// parallel internal loop (bit)
inline void p_internal(long long* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	for (i = 2; i <= sqrtLimit; i++)
		if (!IS_PRIME_BIT(primes, i)) {
			#pragma omp parallel for ordered
			for (j = i * i; j <= limit; j += i)
				MARK_BIT(primes, j);
		}
}

// parallel external loop
inline void p_external(unsigned char* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	#pragma omp parallel for private(i, j) schedule(dynamic)
	for (i = 2; i <= sqrtLimit; i++)
		if (primes[i] == PRIME)
			for (j = i * i; j <= limit; j += i)
				primes[j] = NOTPRIME;
}

// parallel external loop (bit)
inline void p_external(long long* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	#pragma omp parallel for private(i, j) schedule(dynamic) ordered
	for (i = 2; i <= sqrtLimit; i++) {
		#pragma omp ordered
		if (!IS_PRIME_BIT(primes, i))
			for (j = i * i; j <= limit; j += i)
				MARK_BIT(primes, j);
	}
}

// parallel odd numbers only
inline void p_odd(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k;

	for (i = 0; i <= sqrtLimit; i++)
		if (primes[i] == PRIME) {
			j = i * 2 + 3;
			#pragma omp parallel for schedule(dynamic)
			for (k = (j * j - 3) / 2; k <= size; k += j)
				primes[k] = NOTPRIME;
		}
}

// parallel odd numbers only (bit)
inline void p_odd(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k;

	for (i = 0; i <= sqrtLimit; i++)
		if (!IS_PRIME_BIT(primes, i)) {
			j = i * 2 + 3;
			#pragma omp parallel for ordered
			for (k = (j * j - 3) / 2; k <= size; k += j)
				#pragma omp ordered
				MARK_BIT(primes, k);
		}
}

// parallel reorganized loops
inline void p_blocks(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k, l, index;
	long long chunks = CHUNK_SIZE;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh;

	#pragma omp parallel for
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (size) / (chunks + 1);
		chunkHighIndex = (i + 1) * (size) / (chunks + 1) - 1;
		chunkLow = chunkLowIndex + chunkLowIndex + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + 3;

		index = 0;

		do {
			j = index + index + 3;
			k = j * j;

			if (k > chunkLow)
				k = ((k - 3) >> 1) - ((chunkLow - 3) >> 1);
			else {
				k = chunkLow % j;
				if (k != PRIME) {
					if (j > chunkLow % (j + j)) k = (j - k) >> 1;
					if (j < chunkLow % (j + j)) k = j - (k >> 1);
				}
			}

			k += chunkLowIndex;

			for (l = k; l <= chunkHighIndex; l += j)
				primes[l] = NOTPRIME;

			index++;

			while (primes[index] == NOTPRIME)
				index++;

		} while (j * j <= chunkHigh);
	}
}

// parallel reorganized loops (bit)
inline void p_blocks(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k, l, index;
	long long chunks = CHUNK_SIZE;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh;

	#pragma omp parallel for
	for (i = 0; i <= chunks; i++) {
		chunkLowIndex = i * (size) / (chunks + 1);
		chunkHighIndex = (i + 1) * (size) / (chunks + 1) - 1;
		chunkLow = chunkLowIndex + chunkLowIndex + 3;
		chunkHigh = chunkHighIndex + chunkHighIndex + 3;

		index = 0;

		do {
			j = index + index + 3;
			k = j * j;

			if (k > chunkLow)
				k = ((k - 3) >> 1) - ((chunkLow - 3) >> 1);
			else {
				k = chunkLow % j;
				if (k != PRIME) {
					if (j > chunkLow % (j + j)) k = (j - k) >> 1;
					if (j < chunkLow % (j + j)) k = j - (k >> 1);
				}
			}

			k += chunkLowIndex;

			for (l = k; l <= chunkHighIndex; l += j)
				MARK_BIT(primes, l);

			index++;

			while (IS_PRIME_BIT(primes, index))
				index++;

		} while (j * j <= chunkHigh);
	}
}

#endif