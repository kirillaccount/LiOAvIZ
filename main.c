#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

typedef struct {
    int* neighbors;
    int size;
    int loop;
} Edge;

// Функция для вывода матрицы смежности
void print_adjacency_table(int S, int** A, int* deg, int* loop) {
    printf("\nМатрица смежности:\n");
    printf("%-5s", "G");
    for (int j = 0; j < S; ++j)
        printf("%5d", j + 1);
    printf("%8s%8s\n", "loop", "deg");

    for (int i = 0; i < S; ++i) {
        printf("%-5d", i + 1);
        for (int j = 0; j < S; ++j)
            printf("%5d", A[i][j]);
        printf("%8d%8d\n", loop[i], deg[i]);
    }
    printf("\n");
}

// Функция для расчета размера графа по матрице смежности
int calculate_graph_size_from_adjacency(int S, int** A, int* loop) {
    int size = 0;

    // Подсчет обычных ребер (без петель)
    for (int i = 0; i < S; ++i) {
        for (int j = i + 1; j < S; ++j) {
            if (A[i][j]) size++;
        }
    }

    // Добавляем петли
    for (int i = 0; i < S; ++i) {
        if (loop[i]) size++;
    }

    return size;
}

// Функция для построения и вывода матрицы инцидентности
int print_incidence_matrix(int S, int** A, int* loop, int* deg) {
    printf("\n=== ЗАДАНИЕ 2 ===\n");

    // Расчет размера графа по матрице инцидентности
    int edge_count = 0;

    // Считаем ребра без петель
    for (int i = 0; i < S; ++i) {
        for (int j = i + 1; j < S; ++j) {
            if (A[i][j]) edge_count++;
        }
    }

    // Добавляем петли
    for (int i = 0; i < S; ++i) {
        if (loop[i]) edge_count++;
    }

    printf("1. Матрица инцидентности:\n");

    if (edge_count == 0) {
        printf("Граф не содержит ребер.\n");
        return edge_count;
    }

    // Создаем матрицу инцидентности S x edge_count
    int** incidence = malloc(S * sizeof(int*));
    for (int i = 0; i < S; ++i) {
        incidence[i] = calloc(edge_count, sizeof(int));
    }

    // Заполняем матрицу инцидентности
    int current_edge = 0;

    // Заполняем обычные ребра (без петель)
    for (int i = 0; i < S; ++i) {
        for (int j = i + 1; j < S; ++j) {
            if (A[i][j]) {
                incidence[i][current_edge] = 1;
                incidence[j][current_edge] = 1;
                current_edge++;
            }
        }
    }

    // Заполняем петли
    for (int i = 0; i < S; ++i) {
        if (loop[i]) {
            incidence[i][current_edge] = 2; // Петля помечается как 2
            current_edge++;
        }
    }

    // Выводим заголовок матрицы инцидентности
    printf("%-5s", "В\\Р");
    for (int j = 0; j < edge_count; ++j) {
        printf("%3d", j + 1);
    }
    printf("\n");

    for (int i = 0; i < S; ++i) {
        printf("%-5d", i + 1);
        for (int j = 0; j < edge_count; ++j) {
            printf("%3d", incidence[i][j]);
        }
        printf("\n");
    }

    // Освобождаем память
    for (int i = 0; i < S; ++i) {
        free(incidence[i]);
    }
    free(incidence);

    return edge_count;
}

// Функция для вывода сравнения размеров графа
void print_graph_size_comparison(int size_from_adjacency, int size_from_incidence) {
    printf("\n2. Размер графа (количество ребер):\n");
    printf("   - По матрице смежности: %d\n", size_from_adjacency);
    printf("   - По матрице инцидентности: %d\n", size_from_incidence);

    if (size_from_adjacency == size_from_incidence) {
        printf("   ✓ Результаты совпадают!\n");
    }
    else {
        printf("   ⚠ Ошибка: результаты не совпадают!\n");
    }
}

// Функция для поиска специальных вершин
void find_special_vertices(int S, int* deg, int* loop) {
    printf("\n3. Специальные вершины:\n");

    printf("   Изолированные вершины (deg = 0): ");
    int found = 0;
    for (int i = 0; i < S; ++i) {
        if (deg[i] == 0) {
            if (found) printf(", ");
            printf("%d", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    printf("   Концевые вершины (deg = 1, без петли): ");
    found = 0;
    for (int i = 0; i < S; ++i) {
        if (deg[i] == 1 && loop[i] == 0) {
            if (found) printf(", ");
            printf("%d", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");

    printf("   Доминирующие вершины (соединены со всеми остальными): ");
    found = 0;
    for (int i = 0; i < S; ++i) {
        if ((deg[i] - loop[i]) == (S - 1)) {
            if (found) printf(", ");
            printf("%d", i + 1);
            found = 1;
        }
    }
    if (!found) printf("нет");
    printf("\n");
}

int read_row(int S, int* row, int row_index) {
    char buf[4096];
    while (1) {
        printf("Введите %d значений (0 или 1) для строки %d через пробел: ", S, row_index + 1);
        if (!fgets(buf, sizeof(buf), stdin))
            return 0;

        int pos = 0, n = 0, ok = 1;

        while (n < S) {
            while (buf[pos] && isspace((unsigned char)buf[pos])) pos++;
            if (!buf[pos]) { ok = 0; break; }

            if (buf[pos] == '0' || buf[pos] == '1') {
                row[n++] = buf[pos++] - '0';
            }
            else {
                int val, consumed = 0;
                if (sscanf_s(buf + pos, "%d%n", &val, &consumed) == 1) {
                    if (val == 0 || val == 1) {
                        row[n++] = val;
                        pos += consumed;
                    }
                    else { ok = 0; break; }
                }
                else { ok = 0; break; }
            }
        }

        if (ok && n == S) break;
        printf("Неверный ввод. Попробуйте снова.\n");
    }
    return 1;
}

int main(void) {
    setlocale(LC_ALL, "");
    int S;
    printf("Введите число вершин S: ");
    if (scanf_s("%d", &S) != 1 || S <= 0) {
        printf("Неверное значение S.\n");
        return 1;
    }

    int c; while ((c = getchar()) != '\n' && c != EOF);

    int** A = malloc(S * sizeof(int*));
    for (int i = 0; i < S; ++i)
        A[i] = calloc(S, sizeof(int));

    printf("\nПустая таблица (заполните матрицу смежности):\n");
    printf("%-5s", "G");
    for (int j = 0; j < S; ++j)
        printf("%5d", j + 1);
    printf("%8s%8s\n", "loop", "deg");

    for (int i = 0; i < S; ++i) {
        printf("%-5d", i + 1);
        for (int j = 0; j < S; ++j)
            printf("%5s", ".");
        printf("%8d%8d\n", 0, 0);
    }
    printf("\n");

    int* row_buf = malloc(S * sizeof(int));
    for (int i = 0; i < S; ++i) {
        if (!read_row(S, row_buf, i)) {
            printf("Ошибка чтения.\n");
            return 1;
        }
        for (int j = 0; j < S; ++j)
            A[i][j] = row_buf[j];
    }
    free(row_buf);

    int corrected = 0;
    for (int i = 0; i < S; ++i) {
        for (int j = i + 1; j < S; ++j) {
            int v = (A[i][j] || A[j][i]) ? 1 : 0;
            if (A[i][j] != v || A[j][i] != v) corrected = 1;
            A[i][j] = A[j][i] = v;
        }
    }
    if (corrected)
        printf("\nМатрица приведена к симметричному виду (неориентированный граф).\n");

    int* loop = calloc(S, sizeof(int));
    int* deg = calloc(S, sizeof(int));

    for (int i = 0; i < S; ++i) {
        int sum = 0;
        for (int j = 0; j < S; ++j)
            sum += A[i][j];
        loop[i] = A[i][i] ? 1 : 0;
        deg[i] = sum;
    }

    print_adjacency_table(S, A, deg, loop);

    // Расчет размера графа по матрице смежности
    int size_from_adjacency = calculate_graph_size_from_adjacency(S, A, loop);

    // Построение матрицы инцидентности и расчет размера графа по ней
    int size_from_incidence = print_incidence_matrix(S, A, loop, deg);

    // Сравнение размеров графа
    print_graph_size_comparison(size_from_adjacency, size_from_incidence);

    // Поиск специальных вершин
    find_special_vertices(S, deg, loop);

    // Интерактивный анализ вершин
    while (1) {
        int v;
        printf("\n=== ИНТЕРАКТИВНЫЙ АНАЛИЗ ===\n");
        printf("Введите номер вершины (1..%d) для анализа или 0 для выхода: ", S);
        if (scanf_s("%d", &v) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Неверный ввод.\n");
            continue;
        }
        if (v == 0) break;
        if (v < 1 || v > S) {
            printf("Номер вне диапазона.\n");
            continue;
        }

        int idx = v - 1;
        Edge e;

        int neighbors_count = 0;
        for (int j = 0; j < S; ++j)
            if (A[idx][j]) neighbors_count++;

        e.loop = loop[idx];
        int connections = 0;
        for (int j = 0; j < S; ++j) {
            if (A[idx][j]) {
                connections += (j == idx) ? 2 : 1;
            }
        }
        e.size = connections;
        e.neighbors = malloc(neighbors_count * sizeof(int));

        int k = 0;
        for (int j = 0; j < S; ++j)
            if (A[idx][j])
                e.neighbors[k++] = j + 1;

        const char* type;
        if (deg[idx] == 0)
            type = "изолированная";
        else if (deg[idx] == 1 && e.loop == 0)
            type = "концевая";
        else if ((deg[idx] - e.loop) == (S - 1))
            type = "доминирующая";
        else
            type = "обычная";

        printf("\nВершина %d:\n", v);
        printf("  loop = %d\n", e.loop);
        printf("  deg  = %d (петля = 1)\n", deg[idx]);
        printf("  size = %d (петля = 2)\n", e.size);
        printf("  Соседи: ");
        if (neighbors_count == 0) printf("нет");
        else {
            for (int t = 0; t < neighbors_count; ++t) {
                if (t) printf(", ");
                printf("%d", e.neighbors[t]);
            }
        }
        printf("\n  Тип вершины: %s\n", type);

        free(e.neighbors);
    }

    for (int i = 0; i < S; ++i) free(A[i]);
    free(A);
    free(loop);
    free(deg);

    printf("Выход.\n");
    return 0;
}