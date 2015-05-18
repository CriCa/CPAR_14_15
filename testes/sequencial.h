#ifndef SEQUENTIAL
#define SEQUENTIAL

#include <stdio.h>
#include "types.h"

inline void SC_Basic(uchar* primes, ulong limit, ulong sqrtLimit) {
	register ulong k, mark;

	for (k = 2; k <= sqrtLimit; k++)
		if (primes[k] == PRIME)
			for (mark = k * k; mark <= limit; mark += k)
				primes[mark] = COMPOSITE;
}

inline void SC_BasicBits(ulong* primes, ulong limit, ulong sqrtLimit) {
	register ulong k, mark;

	for (k = 2; k <= sqrtLimit; k++)
		if (!TST_BIT(primes, k))
			for (mark = k * k; mark <= limit; mark += k)
				SET_BIT(primes, mark);
}

inline void SC_Opt(uchar* primes, ulong limit, ulong sqrtLimit, ulong size) {
	register ulong i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (primes[i] == 0) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
			mark = (k * k - 3) / 2; // (limite maximo - 3) / 2

			for (; mark <= size; mark += k)
				primes[mark] = 1;
		}
	}
}

inline void SC_OptBits(ulong* primes, ulong limit, ulong sqrtLimit, ulong size) {
	register ulong i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (!TST_BIT(primes, i)) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
			mark = (k * k - 3) / 2; // (limite maximo - 3) / 2

			for (; mark <= size; mark += k)
				SET_BIT(primes, mark);
		}
	}
}

inline void SC_Cached(uchar* primes, ulong limit, ulong sqrtLimit, ulong size) {
	register ulong ind, k, mark, i;
	ulong chunks = 4098;
	ulong chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh, j;

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

			if (mark > chunkLow) {
				mark = ((mark - 3) >> 1) - ((chunkLow - 3) >> 1);
			}
			else {
				mark = chunkLow % k;
				if (mark != 0) {
					if (k > chunkLow % (k + k)) mark = (k - mark) >> 1;
					if (k < chunkLow % (k + k)) mark = k - (mark >> 1);
				}
			}

			mark += chunkLowIndex;

			for (j = mark; j <= chunkHighIndex; j += k) {
				primes[j] = 1;
			}

			ind++;
			while (primes[ind] == 1)
				ind++;

		} while (k * k <= chunkHigh);
	}
}

inline void SC_CachedBits(ulong* primes, ulong limit, ulong sqrtLimit, ulong size) {
	register ulong ind, k, mark, i;
	ulong chunks = 4098;
	ulong chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh, j;

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

			if (mark > chunkLow) {
				mark = ((mark - 3) >> 1) - ((chunkLow - 3) >> 1);
			}
			else {
				mark = chunkLow % k;
				if (mark != 0) {
					if (k > chunkLow % (k + k)) mark = (k - mark) >> 1;
					if (k < chunkLow % (k + k)) mark = k - (mark >> 1);
				}
			}

			mark += chunkLowIndex;

			for (j = mark; j <= chunkHighIndex; j += k) {
				SET_BIT(primes, j);
			}

			ind++;
			while (TST_BIT(primes, ind)){
				ind++;
			}
		} while (k * k <= chunkHigh);
	}
}



#endif