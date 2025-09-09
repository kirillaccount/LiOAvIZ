#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

typedef struct {
	char familia[20];
	char imya[20];
	char fakultet[20];
	int zach_num;
} Student;

void findMaxMinDiff();
void randomInitArray();
void createDynamicArray();
void sumRowsColsMatrix(int rows, int cols);
void searchStudentByName();

int main() {
	setlocale(LC_ALL, "Russian");

	int choice;

	do {
		printf("\nМеню выбора действий:\n"
			"1. Найти разницу между максимумом и минимумом массива\n"
			"2. Инициализировать массив случайными числами\n"
			"3. Создать массив произвольного размера\n"
			"4. Суммировать строки и столбцы матрицы\n"
			"5. Искать студента по фамилии\n"
			"0. Выход\n"
			"\nВаш выбор: ");
		scanf_s("%d", &choice);

		switch (choice) {
		case 1:
			findMaxMinDiff(); break;
		case 2:
			randomInitArray(); break;
		case 3:
			createDynamicArray(); break;
		case 4:
			sumRowsColsMatrix(3, 3); break;
		case 5:
			searchStudentByName(); break;
		case 0:
			printf("Завершение работы...\n");
			break;
		default:
			printf("Некорректный выбор. Повторите попытку.\n");
		}
	} while (choice != 0);

	return 0;
}

// Функция нахождения разницы между максимальным и минимальным элементом массива 1
void findMaxMinDiff() {
	const int N = 5;
	int arr[N] = { 10, 20, 30, 40, 50 };
	int max = arr[0], min = arr[0], diff;

	for (int i = 1; i < N; i++) {
		if (arr[i] > max) max = arr[i];
		else if (arr[i] < min) min = arr[i];
	}

	diff = max - min;
	printf("Максимальное значение: %d\nМинимальное значение: %d\nРазница: %d\n", max, min, diff);
}

// ввести размер массива, заполнить массив рандомными числами от -20 до 10 и найти кол-во неотриц. элементов
// Функция заполнения массива случайными числами 2

void randomInitArray() {
	printf("Введите количество элементов в массиве: ");

	int N;
	int* arr;
	scanf_s("%d", &N);
	arr = (int*)malloc(N * sizeof(int));

	srand(time(NULL));

	for (int i = 0; i < N; i++) {
		arr[i] = rand() % 31 - 20;
	}

	int positiveCount = 0;
	for (int i = 0; i < N; i++) {
		if (arr[i] >= 0) {
			positiveCount++;
		}
	}

	printf("Случайный массив: ");
	for (int i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	printf("Количество положительных чисел: %d\n", positiveCount);

	free(arr);
}

// Функция создания массива произвольного размера 3
void createDynamicArray() {
	int* arr;
	int size, i;

	printf("Введите размер массива: ");
	scanf_s("%d", &size);

	arr = (int*)malloc(size * sizeof(int));
	if (arr == NULL) {
		printf("Ошибка выделения памяти!\n");
		return;
	}

	for (i = 0; i < size; i++) {
		printf("Введите элемент #%d: ", i + 1);
		scanf_s("%d", &arr[i]);
	}

	printf("\nЗаполненный массив: ");
	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	free(arr);
}

// Функция суммирования строк и столбцов матрицы с динамической памятью 4
void sumRowsColsMatrix(int rows, int cols) {
	int** mat = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; ++i) {
		mat[i] = (int*)malloc(cols * sizeof(int));
	}

	int value = 1;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			mat[r][c] = value++;
		}
	}

	int* rowSums = (int*)calloc(rows, sizeof(int));
	int* colSums = (int*)calloc(cols, sizeof(int));

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			rowSums[r] += mat[r][c];
		}
	}

	for (int c = 0; c < cols; c++) {
		for (int r = 0; r < rows; r++) {
			colSums[c] += mat[r][c];
		}
	}

	printf("Матрица (%dx%d):\n", rows, cols);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			printf("%d\t", mat[r][c]);
		}
		printf("\n");
	}

	printf("\nСуммы строк: ");
	for (int r = 0; r < rows; r++) {
		printf("%d ", rowSums[r]);
	}

	printf("\nСуммы столбцов: ");
	for (int c = 0; c < cols; c++) {
		printf("%d ", colSums[c]);
	}
	printf("\n");

	for (int i = 0; i < rows; ++i) free(mat[i]); 
	free(mat);                                   
	free(rowSums);                               
	free(colSums);                               
}

// Функция поиска студента по фамилии 5
void searchStudentByName() {
	Student students[] = {
		{"Ivanov", "Иван", "Инф.", 12345},
		{"Ivanova", "Петра", "Мат.", 54321},
		{"Ivanov", "Сергей", "Физ.", 67890}
	};

	char searchFamilia[20];
	int found = 0;

	printf("Введите фамилию студента для поиска: ");
	scanf_s("%s", searchFamilia, sizeof(searchFamilia));

	for (int i = 0; i < 3; i++) {
		if (strstr(students[i].familia, searchFamilia)) {
			printf("\nСтудент найден:\n"
				"Фамилия: %s\nИмя: %s\nФакультет: %s\nЗачётная книжка: %d\n",
				students[i].familia, students[i].imya,
				students[i].fakultet, students[i].zach_num);

			found = 1;
		}
	}

	if (!found) {
		printf("Студент с указанной фамилией не найден.\n");
	}
}