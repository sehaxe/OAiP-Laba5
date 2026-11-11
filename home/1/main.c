#include <stdio.h>

#include "../common.h"

#define MAX_N 100

static void print_array(const char *title, const int a[], int n)
{
    int i;

    printf("%s\n", title);
    for (i = 0; i < n; i++) printf("%5d", a[i]);
    putchar('\n');
}

static void remove_smaller_prev(int **a, int *n)
{
    int i, k = 1;

    for (i = 1; i < *n; i++)
        if ((*a)[i] >= (*a)[k - 1]) (*a)[k++] = (*a)[i];

    *a = xrealloc(*a, (size_t)k * sizeof **a);
    *n = k;
}

int main(void)
{
    int *a, n, i;
    char prompt[32];

    read_int("Размер массива n (1..100): ", 1, MAX_N, &n);
    a = xmalloc((size_t)n * sizeof *a);

    for (i = 0; i < n; i++) {
        snprintf(prompt, sizeof prompt, "a[%d] = ", i);
        read_value(prompt, &a[i]);
    }

    print_array("Введённый массив:", a, n);
    remove_smaller_prev(&a, &n);
    print_array("Массив после удаления:", a, n);
    printf("Элементов осталось: %d\n", n);

    free(a);
    return 0;
}
