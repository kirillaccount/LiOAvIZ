#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shell(int* items, int count)
{
    int i, j, gap, k;
    int x;
    int a[5] = { 9, 5, 3, 2, 1 };

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (j >= 0) && (x < items[j]); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right)
{
    int i, j;
    int x, y;

    i = left; j = right;
    x = items[(left + right) / 2];

    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left)) j--;

        if (i <= j) {
            y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while (i <= j);

    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int* copy_array(const int* src, int count) {
    int* dest = (int*)malloc(count * sizeof(int));
    if (dest == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
    return dest;
}

void generate_random_array(int* arr, int count) {
    for (int i = 0; i < count; i++) {
        arr[i] = rand() % 1000;
    }
}

double measure_time(void (*sort_func)(int*, int), int* arr, int count) {
    clock_t start, end;
    start = clock();
    sort_func(arr, count);
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double measure_time_qs(void (*sort_func)(int*, int, int), int* arr, int count) {
    clock_t start, end;
    start = clock();
    sort_func(arr, 0, count - 1);
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double measure_time_qsort(int* arr, int count) {
    clock_t start, end;
    start = clock();
    qsort(arr, count, sizeof(int), compare);
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int input_array_size() {
    int size;
    printf("Enter array size for testing (100 - 1000000): ");
    if (scanf_s("%d", &size) != 1) {
        printf("Invalid input. Using default size 10000\n");
        return 10000;
    }

    if (size < 100) {
        printf("Size too small, set to 100\n");
        return 100;
    }
    if (size > 1000000) {
        printf("Size too large, set to 1000000\n");
        return 1000000;
    }
    return size;
}

int main() {
    srand(time(NULL));

    int count;
    char choice;

    do {
        printf("\n=========================================\n");
        printf("      SORTING ALGORITHMS BENCHMARK\n");
        printf("=========================================\n\n");

        count = input_array_size();
        printf("\n");

        if (count > 50000) {
            printf("Warning: Large array size (%d elements)\n", count);
            printf("Testing may take significant time...\n\n");
        }

        int* random_array = (int*)malloc(count * sizeof(int));
        int* ascending_array = (int*)malloc(count * sizeof(int));
        int* descending_array = (int*)malloc(count * sizeof(int));
        int* mixed_array = (int*)malloc(count * sizeof(int));

        if (random_array == NULL || ascending_array == NULL ||
            descending_array == NULL || mixed_array == NULL) {
            printf("Memory allocation error\n");

            if (random_array) free(random_array);
            if (ascending_array) free(ascending_array);
            if (descending_array) free(descending_array);
            if (mixed_array) free(mixed_array);

            return 1;
        }

        printf("Generating test arrays...\n");

        generate_random_array(random_array, count);

        for (int i = 0; i < count; i++) {
            ascending_array[i] = i;



            descending_array[i] = count - i - 1;
        }

        int half = count / 2;
        for (int i = 0; i < half; i++) {
            mixed_array[i] = i;
        }
        for (int i = half; i < count; i++) {
            mixed_array[i] = count - i - 1 + half;
        }

        printf("Testing with array size %d:\n\n", count);

        printf("=== 1. Random Array ===\n");
        int* temp_arr = copy_array(random_array, count);
        double time = measure_time(shell, temp_arr, count);
        printf("Shell Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(random_array, count);
        time = measure_time_qs(qs, temp_arr, count);
        printf("Quick Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(random_array, count);
        time = measure_time_qsort(temp_arr, count);
        printf("Standard qsort: %.6f seconds\n\n", time);
        free(temp_arr);

        printf("=== 2. Ascending Array ===\n");
        temp_arr = copy_array(ascending_array, count);
        time = measure_time(shell, temp_arr, count);
        printf("Shell Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(ascending_array, count);
        time = measure_time_qs(qs, temp_arr, count);
        printf("Quick Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(ascending_array, count);
        time = measure_time_qsort(temp_arr, count);
        printf("Standard qsort: %.6f seconds\n\n", time);
        free(temp_arr);

        printf("=== 3. Descending Array ===\n");
        temp_arr = copy_array(descending_array, count);
        time = measure_time(shell, temp_arr, count);
        printf("Shell Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(descending_array, count);
        time = measure_time_qs(qs, temp_arr, count);
        printf("Quick Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(descending_array, count);
        time = measure_time_qsort(temp_arr, count);
        printf("Standard qsort: %.6f seconds\n\n", time);
        free(temp_arr);

        printf("=== 4. Mixed Array ===\n");
        temp_arr = copy_array(mixed_array, count);
        time = measure_time(shell, temp_arr, count);
        printf("Shell Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(mixed_array, count);
        time = measure_time_qs(qs, temp_arr, count);
        printf("Quick Sort: %.6f seconds\n", time);
        free(temp_arr);

        temp_arr = copy_array(mixed_array, count);
        time = measure_time_qsort(temp_arr, count);
        printf("Standard qsort: %.6f seconds\n\n", time);
        free(temp_arr);

        free(random_array);
        free(ascending_array);
        free(descending_array);
        free(mixed_array);

        printf("Test with different array size? (y/n): ");
        if (scanf_s(" %c", &choice, 1) != 1) {
            choice = 'n';
        }

    } while (choice == 'y' || choice == 'Y');

    printf("Program finished.\n");
    return 0;
}