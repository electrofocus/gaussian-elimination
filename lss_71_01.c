//
// Created by Amir Mullagaliev on 12/16/2020.
//

#include "lss_71_01.h"

#include <math.h>

double const EPS = 1e-25;
extern int flag_d;

size_t lss_memsize_71_01(int n) { return n * sizeof(double); }

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

void swapCols(int n, int k, int idx, double *A, double *cols) {
    int i = 0;
    double tmp;

    // Перестановка колонок

    for (i = 0; i < n; i++) {
        tmp = A[i * n + k];
        A[i * n + k] = A[i * n + idx];
        A[i * n + idx] = tmp;
    }

    // Перестановка номеров колонок

    tmp = cols[k];
    cols[k] = cols[idx];
    cols[idx] = tmp;
}

int lss_71_01(int n, double *A, double *B, double *X, double *cols) {
    int i = 0, k = 0, j = 0, idx = 0;
    double t, sum = 0, divider = 1, multiplier = 0;

    // Запомнить позиции колонок и проинициализировать вектор X

    for (int i = 0; i < n; i++) {
        cols[i] = i;
        X[i] = -1;
    }

    // Прямой ход

    for (k = 0; k < n; k++) {
        idx = maxColIdx(n, k, A);

        // Если в строке все нули

        if (idx == -1) {
            // Если в правой части не ноль

            if (fabs(B[k]) > EPS) {
                if (flag_d) {
                    printf("\nSystem has no solution\n");
                }
                return 1;
            }
            X[k] = 0;
            continue;
        }

        // Выход на последней итерации

        if (k == n - 1) {
            break;
        }

        if (idx != k) {
            swapCols(n, k, idx, A, cols);

            if (flag_d) {
                printf("%d <-> %d\n\n", k, idx);
            }
        }

        if (flag_d) {
            printSystem(n, A, B);
        }

        for (i = k + 1; i < n; i++) {
            multiplier = A[i * n + k] / A[k * n + k];

            for (j = 0; j < n; j++) {
                A[i * n + j] = A[i * n + j] - A[k * n + j] * multiplier;
            }
            B[i] = B[i] - B[k] * multiplier;
        }

        if (flag_d) {
            printSystem(n, A, B);
        }
    }

    // Обратный ход

    for (k = n - 1; k > -1; k--) {
        if (X[k] != 0) {
            sum = 0;
            for (j = k + 1; j < n; j++) {
                sum += A[k * n + j] * X[j];
            }
            X[k] = (B[k] - sum) / A[k * n + k];
        }
    }

    // Перестановка элементов вектора-ответа

    if (flag_d) {
        printf("\ncols: ");
        for (k = 0; k < n; k++) {
            printf("%f ", cols[k]);
        }
        printf("\n  X:");
        for (k = 0; k < n; k++) {
            printf("%f ", X[k]);
        }
        printf("\n");
    }

    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            if ((int)cols[j] == i) {
                t = X[i];
                X[i] = X[j];
                X[j] = t;

                t = cols[i];
                cols[i] = cols[j];
                cols[j] = t;

                break;
            }
        }

        if (flag_d) {
            printf("\ncols: ");
            for (k = 0; k < n; k++) {
                printf("%f ", cols[k]);
            }
            printf("\n  X: ");
            for (k = 0; k < n; k++) {
                printf("%f ", X[k]);
            }
            printf("\n");
        }
    }

    return 0;
}
