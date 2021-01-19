//
// Created by Amir Mullagaliev on 12/16/2020.
//

#include "lss_71_01.h"

#include <math.h>

double const EPS = 1e-8;
extern int flag_debug;

size_t lss_memsize_71_01(int n) { return n * sizeof(double); }

double KahanSum(const double *X, unsigned int n) {
    double sum = 0.0f;
    double y, t;      // Temporary values.
    double c = 0.0f;  // A running compensation for lost low-order bits.
    unsigned int i = 0;

    for (i = 0; i < n; ++i) {
        y = X[i] - c;       // So far, so good: c is zero.
        t = sum + y;        // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y;  // (t - sum) recovers the high-order part of y; subtracting y recovers -(low part of y)
        sum = t;            // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!
    }

    return sum;
}

int maxColIdx(int n, int k, double *A) {
    int j = 0, colIdx = -1;
    double max = 0;

    for (j = k; j < n; j++) {
        if (fabs(A[k * n + j]) - fabs(max) > EPS) {
            max = fabs(A[k * n + j]);
            colIdx = j;
        }
    }

    return colIdx;
}

void swapCols(int n, int a, int b, double *A, int *colsOrder) {
    int i = 0;
    double tmp;

    // Перестановка колонок
    for (i = 0; i < n; i++) {
        tmp = A[i * n + a];
        A[i * n + a] = A[i * n + b];
        A[i * n + b] = tmp;
    }

    // Перестановка номеров колонок
    tmp = colsOrder[a];
    colsOrder[a] = colsOrder[b];
    colsOrder[b] = tmp;
}

void swapRows(int n, int a, int b, double *A, double *B) {
    int j = 0;
    double tmp;

    for (j = 0; j < n; j++) {
        tmp = A[a * n + j];
        A[a * n + j] = A[b * n + j];
        A[b * n + j] = tmp;
    }

    tmp = B[a];
    B[a] = B[b];
    B[b] = tmp;
}

void printColsOrder(int n, int *colsOrder) {
    int k;

    printf("\ncols: ");
    for (k = 0; k < n; k++) {
        printf("%d ", colsOrder[k]);
    }
    printf("\n");
}

void printX(int n, double *X) {
    int k;

    printf("\n   X: ");
    for (k = 0; k < n; k++) {
        printf("%1.3lf\t", X[k]);
    }
    printf("\n");
}

int lss_71_01(int n, double *A, double *B, double *X, double *tmp) {
    int i = 0, k = 0, j = 0, idx = 0, last = n;
    int *colsOrder = (int *)tmp;
    double t, sum = 0, multiplier = 0;

    // Запомнить позиции колонок и проинициализировать вектор X
    for (int i = 0; i < n; i++) {
        colsOrder[i] = i;
    }

    // Прямой ход
    for (k = 0; k < last; k++) {
        idx = maxColIdx(n, k, A);

        // Если слева в строке все нули
        if (idx == -1) {
            // Если в правой части не ноль
            if (fabs(B[k]) > EPS) {
                if (flag_debug) {
                    printf("\nSystem has no solution.\n");
                }
                return 1;  // нет решения
            }

            swapRows(n, k, last - 1, A, B);
            k--;
            last--;

            if (flag_debug) {
                printf("\nSwap rows: %d <-> %d\n", k + 1, last - 1);
                printSystem(n, A, B);
            }
            continue;
        }

        if (idx != k) {
            swapCols(n, k, idx, A, colsOrder);

            if (flag_debug) {
                printf("\nSwap columns: %d <-> %d\n", k, idx);
                printSystem(n, A, B);
            }
        }

        // Главная часть итерации
        for (i = k + 1; i < last; i++) {
            multiplier = A[i * n + k] / A[k * n + k];

            for (j = k; j < n; j++) {
                A[i * n + j] = A[i * n + j] - A[k * n + j] * multiplier;
            }
            B[i] = B[i] - B[k] * multiplier;
        }

        if (flag_debug) {
            printSystem(n, A, B);
        }
    }

    double *temp = malloc(n * sizeof(double));

    // Обратный ход
    for (k = n - 1; k > -1; k--) {
        sum = 0;
        for (j = 0; j < n; j++) {
            temp[j] = A[k * n + j] * X[j];
        }
        // for (j = k + 1; j < n; j++) {
        //     sum += A[k * n + j] * X[j];
        // }
        sum = KahanSum(temp, n);

        if (fabs(A[k * n + k]) > EPS) {
            X[k] = (B[k] - sum) / A[k * n + k];
        } else {
            X[k] = 0;
        }

        if (flag_debug) {
            printX(n, X);
        }
    }

    // Перестановка элементов вектора-ответа
    if (flag_debug) {
        printColsOrder(n, colsOrder);
        printX(n, X);
    }

    for (i = 0; i < n; i++) {
        if (colsOrder[i] == i) {
            continue;
        }
        for (j = i; j < n; j++) {
            if (colsOrder[j] == i) {
                t = X[i];
                X[i] = X[j];
                X[j] = t;

                t = colsOrder[i];
                colsOrder[i] = colsOrder[j];
                colsOrder[j] = t;

                break;
            }
        }

        if (flag_debug) {
            printColsOrder(n, colsOrder);
            printX(n, X);
        }
    }

    return 0;
}
