#include <cstdlib>

#include "parallel_mpi.h"

// methods
#define MPI 0
#define MPI_SEED 1
#define MPI_BLOCKS 2

#define MAX_METHOD 2

#define EXP_LOWER_LIMIT 25
#define EXP_UPPER_LIMIT 32

string getMethod(int method) {
	switch (method)
	{
	case MPI:
		return "MPI basic";
	case MPI_SEED:
		return "MPI seed";
	case MPI_BLOCKS:
		return "MPI blocks";
	default:
		return "ERROR";
	}
}

#define N_REP 3

int main(int argc, char **argv) {
	int method, exponent, size, rank;
	long long limit, sqrtLimit;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (method = 1; method <= MAX_METHOD; method++) {
		if (rank == MASTER)
			cout << "Testing method: " << getMethod(method).c_str() << endl;

		for (exponent = EXP_LOWER_LIMIT; exponent <= EXP_UPPER_LIMIT; exponent++) {
			if (rank == MASTER)
				cout << "Testing n = " << exponent << endl;

			double avgTime = 0;

			limit = (long long)pow(2.0, exponent);
			sqrtLimit = (long long)sqrt(limit);

			for (int n = 0; n < N_REP; n++) {
				switch (method)
				{
				case MPI:
					avgTime += mpi_basic(rank, size, limit, sqrtLimit);
					break;
				case MPI_SEED:
					avgTime += mpi_seed(rank, size, limit, sqrtLimit);
					break;
				case MPI_BLOCKS:
					avgTime += mpi_blocks(rank, size, limit, sqrtLimit);
					break;
				}
			}

			if (rank == MASTER)
				cout << "Average Time: " << avgTime / N_REP << " secs" << endl << endl;
		}
	}

	MPI_Finalize();

	return 0;
}
