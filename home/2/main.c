#include <stdio.h>
#include <stdlib.h>

#include "../common.h"

static void fill_ragged(int ***a, int **len, int *n)
{
    int rows, mode, i, j;

    read_int("Количество строк (1..20): ", 1, 20, &rows);
    read_int("Заполнение: 1 - вручную, 2 - случайно: ", 1, 2, &mode);

    *a = xmalloc((size_t)rows * sizeof **a);
    *len = xmalloc((size_t)rows * sizeof **len);
    *n = rows;

    for (i = 0; i < rows; i++) {
        if (mode == 1) {
            int cap = 4, size = 0, v;
            int *row = xmalloc((size_t)cap * sizeof *row);

            for (;;) {
                char prompt[48];
                snprintf(prompt, sizeof prompt, "a[%d][%d] (0 - конец): ", i, size);
                read_value(prompt, &v);
                if (v == 0) break;
                if (v < 0) {
                    puts("Ошибка: нужно натуральное число.");
                    continue;
                }
                if (size == cap) {
                    cap *= 2;
                    row = xrealloc(row, (size_t)cap * sizeof *row);
                }
                row[size++] = v;
            }

            if (size == 0) {
                free(row);
                row = NULL;
            } else {
                row = xrealloc(row, (size_t)size * sizeof *row);
            }
            (*a)[i] = row;
            (*len)[i] = size;
        } else {
            int size = rand() % 8 + 1;
            int *row = xmalloc((size_t)size * sizeof *row);

            for (j = 0; j < size; j++) row[j] = rand() % 99 + 1;
            (*a)[i] = row;
            (*len)[i] = size;
        }
    }
}

static void print_ragged(int **a, const int *len, int n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        printf("Строка %d (%d):", i, len[i]);
        for (j = 0; j < len[i]; j++) printf("%4d", a[i][j]);
        putchar('\n');
    }
}

static void duplicate_big(int **a, int *len, int n, int k)
{
    int i, j, extra, new_len, w;

    for (i = 0; i < n; i++) {
        extra = 0;
        for (j = 0; j < len[i]; j++)
            if (a[i][j] > k) extra++;
        if (extra == 0) continue;

        new_len = len[i] + extra;
        int *row = xmalloc((size_t)new_len * sizeof *row);
        w = 0;
        for (j = 0; j < len[i]; j++) {
            row[w++] = a[i][j];
            if (a[i][j] > k) row[w++] = a[i][j];
        }
        free(a[i]);
        a[i] = row;
        len[i] = new_len;
    }
}

static void free_ragged(int **a, int *len, int n)
{
    int i;

    for (i = 0; i < n; i++) free(a[i]);
    free(len);
    free(a);
}

int main(void)
{
    int **a, *len, n, k;

    read_value("K: ", &k);
    fill_ragged(&a, &len, &n);

    puts("Введённый массив:");
    print_ragged(a, len, n);

    duplicate_big(a, len, n, k);
    puts("После дублирования чисел больше K:");
    print_ragged(a, len, n);

    free_ragged(a, len, n);
    return 0;
}
