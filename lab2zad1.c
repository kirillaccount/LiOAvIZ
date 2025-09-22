#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4000

int main(void)
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    clock_t start_all;
    float duration_all;
    start_all = clock();

    int** a = (int**)malloc(SIZE * sizeof(int*));
    int** b = (int**)malloc(SIZE * sizeof(int*));
    int** c = (int**)malloc(SIZE * sizeof(int*));

    for (int i = 0; i < SIZE; i++) {
        a[i] = (int*)malloc(SIZE * sizeof(int));
        b[i] = (int*)malloc(SIZE * sizeof(int));
        c[i] = (int*)malloc(SIZE * sizeof(int));
    }

    srand(time(NULL));

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            a[i][j] = rand() % 100 + 1;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            b[i][j] = rand() % 100 + 1;
        }
    }

    clock_t start;
    float duration;
    start = clock();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int elem_c = 0;
            for (int r = 0; r < SIZE; r++) {
                elem_c += a[i][r] * b[r][j];
            }
            c[i][j] = elem_c;
        }
    }

    duration = (float)(clock() - start) / CLOCKS_PER_SEC;
    duration_all = (float)(clock() - start_all) / CLOCKS_PER_SEC;

    printf("Matrix multiplication execution time: %f seconds\n", duration);
    printf("Total execution time: %f seconds\n", duration_all);

    for (int i = 0; i < SIZE; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}