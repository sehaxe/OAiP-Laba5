#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

static void fill_matrix(int ***a, int *n, int *m)
{
    int i, j, mode;

    read_int("Строк n (1..20): ", 1, 20, n);
    read_int("Столбцов m (1..20): ", 1, 20, m);
    read_int("Заполнение: 1 - вручную, 2 - случайно: ", 1, 2, &mode);

    *a = xmalloc((size_t)*n * sizeof **a);
    for (i = 0; i < *n; i++) {
        (*a)[i] = xmalloc((size_t)*m * sizeof ***a);
        for (j = 0; j < *m; j++) {
            if (mode == 1) {
                char prompt[32];
                snprintf(prompt, sizeof prompt, "a[%d][%d] = ", i, j);
                read_value(prompt, &(*a)[i][j]);
            } else {
                (*a)[i][j] = rand() % 101 - 50;
            }
        }
    }
}

static void print_matrix(int **a, int n, int m)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) printf("%5d", a[i][j]);
        putchar('\n');
    }
}

static int find_saddle(int **a, int n, int m, int *row, int *col)
{
    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            int min_col = 1, max_row = 1;
            for (k = 0; k < n; k++)
                if (a[k][j] < a[i][j]) min_col = 0;
            for (k = 0; k < m; k++)
                if (a[i][k] > a[i][j]) max_row = 0;
            if (min_col && max_row) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
    return 0;
}

static void delete_col(int ***a, int n, int *m, int col)
{
    int i, j, new_m = *m - 1;

    for (i = 0; i < n; i++) {
        int *row = new_m > 0 ? xmalloc((size_t)new_m * sizeof *row) : NULL;

        for (j = 0; j < col; j++) row[j] = (*a)[i][j];
        for (j = col; j < new_m; j++) row[j] = (*a)[i][j + 1];

        free((*a)[i]);
        (*a)[i] = row;
    }
    *m = new_m;
}

static void free_matrix(int **a, int n)
{
    int i;

    for (i = 0; i < n; i++) free(a[i]);
    free(a);
}

int main(void)
{
    int **a, n, m, row, col;

    fill_matrix(&a, &n, &m);
    puts("Введённая матрица:");
    print_matrix(a, n, m);

    if (find_saddle(a, n, m, &row, &col)) {
        printf("Седловая точка: a[%d][%d] = %d\n", row, col, a[row][col]);
        delete_col(&a, n, &m, col);
        printf("Матрица после удаления столбца %d:\n", col);
        if (m == 0)
            puts("Столбцов не осталось.");
        else
            print_matrix(a, n, m);
    } else {
        puts("Седловых точек нет.");
    }

    free_matrix(a, n);
    return 0;
}
