//
// Created by Amir Mullagaliev on 12/16/2020.
//

#include "lss_71_01.h"

#include <math.h>

double const EPS = 1e-25;

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

    // Переставить колонки местами

    for (i = 0; i < n; i++) {
        tmp = A[i * n + k];
        A[i * n + k] = A[i * n + idx];
        A[i * n + idx] = tmp;
    }

    // Переставить номера колонок

    i = cols[k];
    cols[k] = cols[idx];
    cols[idx] = i;
}

int lss_71_01(int n, double *A, double *B, double *X, double *cols) {
    int i = 0, k = 0, j = 0, idx = 0;
    double t, sum = 0;

    // Запомнить позиции колонок и проинициализировать вектор X

    for (int i = 0; i < n; i++) {
        cols[i] = i;
        X[i] = -1;
    }

    // Прямой ход

    for (k = 0; k < n - 1; k++) {
        idx = maxColIdx(n, k, A);

        if (idx == -1) {
            if (B[k] > EPS) {
                return 1;
            }
            X[k] = 0;
        }

        if (idx != k) {
            swapCols(n, k, idx, A, cols);
            printf("swap %d with %d\n", k, idx);
        }

        printSystem(n, A, B);

        t = A[k * n + k];
        for (j = 0; j < n; j++) {
            A[k * n + j] = A[k * n + j] / t;
        }
        B[k] = B[k] / t;

        for (i = k + 1; i < n; i++) {
            t = A[i * n + k];  // / A[k * n + k];

            for (j = 0; j < n; j++) {
                A[i * n + j] = A[i * n + j] - A[k * n + j] * t;
            }
            B[i] = B[i] - B[k] * t;
        }

        printSystem(n, A, B);
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

    for (j = 0; j < n; j++) {
        printf("%1.9lf ", cols[j]);
    }
    printf("\n\n");

    // Перестановка элементов вектора-ответа

    for (j = 0; j < n; j++) {
        t = X[(int)cols[j]];
        X[(int)cols[j]] = X[j];
        X[j] = t;

        t = cols[(int)cols[j]];
        cols[(int)cols[j]] = cols[j];
        cols[j] = t;
    }
}
