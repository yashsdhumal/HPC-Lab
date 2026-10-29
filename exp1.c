#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000 // 100 Million Elements

int main() {
    int *A = (int *)malloc(N * sizeof(int));
    int *B = (int *)malloc(N * sizeof(int));
    int *C = (int *)malloc(N * sizeof(int));

    // Initialize arrays A and B
    for (int i = 0; i < N; i++) {
        A[i] = i % 100;
        B[i] = (i % 100) * 2;
    }

    // Parallelize array addition
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        C[i] = A[i] + B[i];
    }

    printf("Vector addition completed successfully for N = %d elements.\n", N);

    free(A); free(B); free(C);
    return 0;
}