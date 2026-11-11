# OAiP-Laba5

Лабораторная работа №5 по ОАиП (БГУИР), тема «Динамическое выделение
памяти», вариант 5. Три задачи на Си с `malloc`/`realloc` и готовый отчёт
без блок-схем: условия, ход решения, листинги и реальные прогоны.

## Быстрый запуск

```bash
git clone https://github.com/sehaxe/OAiP-Laba5
cd OAiP-Laba5/home
make            # соберёт 1/main, 2/main, 3/main
./1/main
./2/main
./3/main
```

Нужен только `gcc`. Готовый отчёт: `home/report/main.pdf`.

## Задачи

| № | Условие (вариант 5) | Файл |
|----|---------------------|------|
| 1 | Удалить элементы, которые меньше предыдущего | `home/1/main.c` |
| 2 | В рваном двумерном массиве продублировать числа больше K | `home/2/main.c` |
| 3 | Найти седловую точку и удалить её столбец | `home/3/main.c` |

## Пересборка отчёта

```bash
cd home
make report     # gcc -> term.json -> report/main.pdf
```

Нужны `gcc`, `python3` и `typst`. Блок-схем в этой работе нет.

Общее — `home/common.h`: `read_int`, `read_value`, `xmalloc`, `xrealloc`.
Компиляция: `gcc -std=c11 -Wall -Wextra -Werror -pedantic`.
