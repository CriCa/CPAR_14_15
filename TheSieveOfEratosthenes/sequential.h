#ifndef SEQUENTIAL
#define SEQUENTIAL

#include "common.h"

// sequential fast marking
inline void s_fastMarking(unsigned char* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	for (i = 2; i <= sqrtLimit; i++)
		if (primes[i] == PRIME)
			for (j = i * i; j <= limit; j += i)
				primes[j] = NOTPRIME;
}

// sequential fast marking (bit)
inline void s_fastMarking(long long* primes, long long limit, long long sqrtLimit) {
	register long long i, j;

	for (i = 2; i <= sqrtLimit; i++)
		if (!IS_PRIME_BIT(primes, i))
			for (j = i * i; j <= limit; j += i)
				MARK_BIT(primes, j);
}

// sequential odd numbers only
inline void s_odd(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k;

	for (i = 0; i <= sqrtLimit; i++)
		if (primes[i] == PRIME) {
			j = i * 2 + 3;
			k = (j * j - 3) / 2;
			for (; k <= size; k += j)
				primes[k] = NOTPRIME;
		}
}

// sequential odd numbers only (bit)
inline void s_odd(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k;

	for (i = 0; i <= sqrtLimit; i++)
		if (!IS_PRIME_BIT(primes, i)) {
			j = i * 2 + 3;
			k = (j * j - 3) / 2;
			for (; k <= size; k += j)
				MARK_BIT(primes, k);
		}
}

// sequential reorganized loops
inline void s_blocks(unsigned char* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k, l, index;
	long long chunks = CHUNK_SIZE;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh;

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

// sequential reorganized loops (bit)
inline void s_blocks(long long* primes, long long limit, long long sqrtLimit, long long size) {
	register long long i, j, k, l, index;
	long long chunks = CHUNK_SIZE;
	long long chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh;

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
				if (k != 0) {
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