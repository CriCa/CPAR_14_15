#ifndef SEQUENTIAL
#define SEQUENTIAL

#include "common.h"

inline void s_base(uchar* primes, ulong limit, ulong sqrtLimit) {
	register ulong k, mark;

	for (k = 2; k <= sqrtLimit; k++)
		if (primes[k] == PRIME)
			for (mark = k * k; mark <= limit; mark += k)
				primes[mark] = COMPOSITE;
}

#endif