#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;

void singleCore(int* numbers, int n, int k, int mark, int sqrtLimit) {
	for (k = 2; k <= n - 1; k++)
		numbers[k] = 0;

	for (k = 2; k <= sqrtLimit; k++) {
		if (!numbers[k]) {
			for (mark = k + 1; mark <= n - 1; mark++) {
				if (mark%k == 0) {
					numbers[mark] = 1;
				}
			}
		}
	}
}

void openMP(int* numbers, int n, int k, int mark, int sqrtLimit) {
#pragma omp parallel for
	for (k = 2; k <= n - 1; k++)
		numbers[k] = 0;

	for (k = 2; k <= sqrtLimit; k++) {
		if (!numbers[k]) {
#pragma omp parallel for
			for (mark = k + 1; mark <= n - 1; mark++) {
				if (mark%k == 0) {
					numbers[mark] = 1;
				}
			}
		}
	}
}

int main()
{
	omp_set_dynamic(0);
	omp_set_num_threads(12);

	int *numbers, k = 0, mark = 0;
	int n = 10000000;
	int sqrtLimit = sqrt(n);
	numbers = (int*)malloc(n * sizeof(int));

	int method = 1;
	clock_t t1, t2;
	double elapsed_secs;
		
		t1 = clock();
		singleCore(numbers, n, k, mark, sqrtLimit);
		t2 = clock();
		elapsed_secs = double(t2 - t1);
	
		cout << elapsed_secs << endl;

		t1 = clock();
		openMP(numbers, n, k, mark, sqrtLimit);
		t2 = clock();
		elapsed_secs = double(t2 - t1);

		cout << elapsed_secs << endl;

	/*for (k = 2; k <= n - 1; k++) {
	if (numbers[k] == 0) {
	printf("%d ", k);
	}
	}
	printf("\n");*/

		system("PAUSE");

	free(numbers);
}