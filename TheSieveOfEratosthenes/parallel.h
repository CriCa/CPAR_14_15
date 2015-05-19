#ifndef PARALLEL
#define PARALLEL

#include <omp.h>
#include "common.h"

inline void p_internal(uchar* primes, llong limit, llong sqrtLimit) {
	register llong k, mark;

	for (k = 2; k <= sqrtLimit; k++) {
		if (primes[k] == 0) {
#pragma omp parallel for
			for (mark = k * k; mark <= limit; mark += k)
				primes[mark] = 1;
		}
	}
}

inline void openMPBits(llong* primes, llong limit, llong sqrtLimit) {
	register llong k, mark;

	for (k = 2; k <= sqrtLimit; k++) {
		if (!TST_BIT(primes, k)) {
#pragma omp parallel for  ordered
			for (mark = k * k; mark <= limit; mark += k)
#pragma omp ordered
				SET_BIT(primes, mark);
		}
	}
}

inline void openMP2(uchar* primes, llong limit, llong sqrtLimit) {
	register llong k, mark;

#pragma omp parallel for private(k, mark) schedule(dynamic)
	for (k = 2; k <= sqrtLimit; k++) {
		if (primes[k] == 0) {
			for (mark = k * k; mark <= limit; mark += k)
				primes[mark] = 1;
		}
	}
}

inline void openMP2Bits(llong* primes, llong limit, llong sqrtLimit) {
	register llong k, mark;

#pragma omp parallel for private(k, mark) schedule(dynamic) ordered
	for (k = 2; k <= sqrtLimit; k++) {
#pragma omp ordered
		if (!TST_BIT(primes, k)) {
			for (mark = k * k; mark <= limit; mark += k)
				SET_BIT(primes, mark);
		}
	}
}

inline void openMPOpt(uchar* primes, llong limit, llong sqrtLimit, llong size) {
	register llong i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (primes[i] == 0) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
#pragma omp parallel for schedule(dynamic)
			for (mark = (k * k - 3) / 2; mark <= size; mark += k)
				primes[mark] = 1;
		}
	}
}

inline void openMPOptBits(llong* primes, llong limit, llong sqrtLimit, llong size) {
	register llong i, k, mark;

	for (i = 0; i <= sqrtLimit; i++) {
		if (!TST_BIT(primes, i)) {
			k = i * 2 + 3; //semente atual transferido para o número real - 3 + 2*k
#pragma omp parallel for ordered
			for (mark = (k * k - 3) / 2; mark <= size; mark += k)
#pragma omp ordered
				SET_BIT(primes, mark);
		}
	}
}

inline void openMPOptCacheBits(llong* primes, llong limit, llong sqrtLimit, llong size) {
	register llong ind, k, mark, i;
	llong chunks = 100;
	llong chunkLowIndex, chunkHighIndex, chunkLow, chunkHigh, j;

	//proccess own block
#pragma omp parallel for
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

			//#pragma omp parallel for
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