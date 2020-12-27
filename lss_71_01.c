//
// Created by Amir Mullagaliev on 12/16/2020.
//

#include "lss_71_01.h"

int lss_71_01(int n, double *A, double *B, double *X, double *columns) {
    int i = 0, k = 0, j = 0;
    double t;

    for (int i = 0; i < n; i++) {
        columns[i] = i;
    }

    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            A[k * n + j] = A[k * n + j] / A[k * n + k];
        }
        B[k] = B[k] / A[k * n + k];

        for (int i = k + 1; i < n; i++) {
            t = A[i * n + k] / A[k * n + k];

            for (int j = 0; j < n; j++) {
                A[i * n + j] = A[i * n + j] - A[k * n + j] * t;
            }
            B[i] = B[i] - B[k] * t;
        }
    }
}
