#ifndef OAIP_COMMON_H
#define OAIP_COMMON_H

#include <stdio.h>
#include <stdlib.h>

static inline void *xmalloc(size_t size)
{
    void *p = malloc(size);

    if (p == NULL && size != 0) {
        fputs("Не хватило памяти.\n", stderr);
        exit(EXIT_FAILURE);
    }
    return p;
}

static inline void *xrealloc(void *p, size_t size)
{
    void *q = realloc(p, size);

    if (q == NULL && size != 0) {
        fputs("Не хватило памяти.\n", stderr);
        exit(EXIT_FAILURE);
    }
    return q;
}

static inline void read_int(const char *prompt, int lo, int hi, int *value)
{
    char line[128], extra;
    int v;

    while (1) {
        fputs(prompt, stdout);
        if (fgets(line, sizeof line, stdin) == NULL) exit(EXIT_FAILURE);
        if (sscanf(line, " %d %c", &v, &extra) == 1 && lo <= v && v <= hi) {
            *value = v;
            return;
        }
        printf("Ошибка: нужно целое число от %d до %d.\n", lo, hi);
    }
}

static inline void read_value(const char *prompt, int *value)
{
    char line[128], extra;
    int v;

    while (1) {
        fputs(prompt, stdout);
        if (fgets(line, sizeof line, stdin) == NULL) exit(EXIT_FAILURE);
        if (sscanf(line, " %d %c", &v, &extra) == 1) {
            *value = v;
            return;
        }
        puts("Ошибка: нужно целое число.");
    }
}

#endif
