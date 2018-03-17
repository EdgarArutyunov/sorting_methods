#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {TST = 4};/// ...
enum {QUICK, BUBBLE};/// ...
enum {ASC, DEC, R3, R4};/// ...

int comp[2], exc[2], N[TST] = {10, 100, 1000, 10000}; /// comp[0], exc[0] отвечают за кол-во сравнений и обменов у QSort
///comp[1], exc[1] соответственно у Bsort

char *arr_infa[4] = {
    "The inf for Ascend_Array: \n",
    "The inf for Decreas_Array: \n",
    "The inf for Rand_3_Array: \n",
    "The inf for Rand_4_Array: \n"
};

char *sort_print[2] = {
    "    QSORT N = %d, Comp = %d, Exc = %d\n",
    "    BSORT N = %d, Comp = %d, Exc = %d\n"
};

void (*gen_array[4])(int *, int); /// указатели на функции, генерирующие массивы(возр, убыв и  2 рандома)
void (*sort[2])(int *, int); /// указатели на сортировки(Quick и Bubble)

int cmp(int a, int b, int type_sort) { /// Компаратор. type_sort - вы-вшая нас сортировка
    if (a < 0) a *= -1;
    if (b < 0) b *= -1;
    if (type_sort != -1) ++comp[type_sort];
    return a < b;
}

void swap(int *a, int *b, int type_sort) { /// Обмен объектов, на которые ссылаются *a и *b и увелич. кол-во обменов у вы-вшей сорт-ки
    exc[type_sort]++; ///добавляем единичку к обмену той сортировки, которая нас собственно вызвала
    int c = *a; /// тут все и так понятно
    *a = *b;
    *b = c;
}

void Qsort(int* a, int n) { /// Quck Sort a[..] размера n.
    if (n <= 1) return;
    int *L = a, *R = a + (n - 1); /// L - указывает на первый элемент, R - на последний

    int val = *(L + n / 2); /// опорный элемент
    int *l = L, *r = R;
    while (l < r) {
        while (l <= r && cmp(*l, val, QUICK) ) ++l; /// ищу >= опорного. Слева направо
        while (l <= r && cmp(val, *r, QUICK) ) --r; /// ищу <= опорного. Справа налево
        if (l <= r) {
            if (l != r)
                swap(l, r, QUICK);

            ++l, --r;
        }
    }
    Qsort(L, r - L + 1);
    Qsort(l, R - l + 1);
}

void Bsort(int *a, int n) { /// Bubble Sort a[..] размера n.
    while (1) { ///ну вот так вот
        int exc_was = 0; /// отвечает, был ли у нас обмен
        for (int i = 0; i < n - 1; ++i) {
            if (cmp(a[i + 1], a[i], BUBBLE)) {
                swap(a + i, a + i + 1, BUBBLE);
                exc_was = 1;
            }
        }
        if (!exc_was)
            break ;
    }
}

void gen_ascend(int *a, int n) { /// иниц. a[..] как возр. посл-сть
    for (int i = 0; i < n; ++i)
        a[i] = i;
}

void gen_decreas(int *a, int n) { /// иниц. a[..] как убыв. посл-сть
    for (int i = 0; i < n; ++i)
        a[i] = n - i;
}

void gen_rand3(int *a, int n) { /// иниц. a[..] ранд. значениями
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        a[i] = (rand() % 3533) * (rand() % 1787);
        a[i] *= ((rand() % 2801)&1 ? -1 : 1);
    }
}

void gen_rand4(int *a, int n) { /// иниц. a[..] ранд. значениями
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        a[i] = (rand() % 3571) * (rand() % 2411); /// генерируем модуль
        a[i] *= ((rand() % 2357)&1 ? -1 : 1); /// генерируем знак
    }
}

int check(int *a, int n) {
    for (int i = 0; i + 1 < n; ++i) {
        if (cmp(a[i + 1], a[i], -1)) {
            printf("\n\nAAAA!!! %d %d\n\n", a[i], a[i + 1]);
            return 0;
        }
    }

    return 1;
}

void print_inf(int type_ar, int type_sort) { /// печать данных для type_ar - массива и type_sort - сортировки
    int *a = malloc(48);
    for (int i = 0; i < TST; ++i) { /// идем по тестам N = 10, 100, 1000, 10000
        a = realloc(a, 4 * N[i]); /// выделяем сколько нужно для
        comp[0] = comp[1] = exc[0] = exc[1] = 0; /// обнуляем результаты с предыдущих экспериментов
        gen_array[type_ar](a, N[i]);
        sort[type_sort](a, N[i]);
        printf(sort_print[type_sort], N[i], comp[type_sort], exc[type_sort]);
        if (check(a, N[i]))
            printf("    [..OK..] ====> The sort was correct!\n\n");
        else {
            printf("    [!!ERROR!!] ====> Array isn't sorted!!!\n\n");
            if (N[i] == 10) {
                for (int j = 0; j < 10; ++j)
                    printf("(%d) \n", a[j]);

                printf("\n");
            }
        }
    }
    printf("\n");
}

void print(int type_ar) { ///печатает
    printf(arr_infa[type_ar]);
    print_inf(type_ar, QUICK);
    print_inf(type_ar, BUBBLE);
}

void init_func() { /// иниц. указатели на функции
    gen_array[ASC] = &gen_ascend;
    gen_array[DEC] = &gen_decreas;
    gen_array[R3] = &gen_rand3;
    gen_array[R4] = &gen_rand4;

    sort[QUICK] = &Qsort;
    sort[BUBBLE] = &Bsort;
}

void print_ar(int *a, int n) {
    for (int i = 0; i < n; ++i)
        printf("---->%d\n", a[i]);
}

int main() { ///...
    init_func();
    for (int type_ar = 0; type_ar < 4; ++type_ar) {
        printf(arr_infa[type_ar]);
        for (int type_sort = 0; type_sort < 2; ++type_sort) {
            print_inf(type_ar, type_sort);
        }
    }
    return 0;
}
