//
// Created by Amir Mullagaliev on 12/16/2020.
//
// #1. Метод Гаусса с выбором главного элемента по строке
//

#include "lss_71_01.h"

int flag_d = 0, flag_e = 0, flag_p = 0, flag_t = 0, flag_h = 0, flag_q = 0, max_iter = 0;

int stringLength(char const *input) {
    int length = 0;
    while (input[length] != '\0') {
        length++;
    }
    return length;
}

int validateFile(char *filename) {
    printf("%s", filename);
    FILE *f = fopen(filename, "rb");
    int result = f != NULL;
    return result;
}

int validateParams(int argc, char **argv) {
    int i, count = 0;
    if (argc == 1) {
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && stringLength(argv[i]) == 2) {
            switch (argv[i][1]) {
                case 'd': {
                    flag_d = 1;
                    break;
                }
                case 'h': {
                    flag_h = 1;
                    break;
                }
                case '?': {
                    flag_q = 1;
                }
                case 'e': {
                    flag_e = 1;
                    break;
                }
                case 'p': {
                    flag_p = 1;
                    break;
                }
                case 't': {
                    flag_t = 1;
                    break;
                }
                default: {
                    return 2;
                }
            }
        } else {
            count++;
        }
    }

    if (count > 2) {
        return 1;
    }
    return 0;
}

int readInput(char *inputFile, double **A, double **B, int *n) {
    int i, checkInput;
    FILE *input = fopen(inputFile, "r");

    checkInput = fscanf(input, "%d", n);
    if (checkInput == EOF) {
        return 6;
    }

    if (checkInput == 0 || *n <= 0) {
        return 7;
    }

    *A = malloc((*n) * (*n) * sizeof(double));
    *B = malloc((*n) * sizeof(double));

    for (i = 0; i < (*n) * (*n); i++) {
        checkInput = fscanf(input, "%lf", (*A + i));
        if (checkInput == EOF) {
            return 8;
        }
        if (checkInput == 0) {
            return 9;
        }
    }

    for (i = 0; i < (*n); i++) {
        checkInput = fscanf(input, "%lf", (*B + i));
        if (checkInput == EOF) {
            return 8;
        }
        if (checkInput == 0) {
            return 9;
        }
    }

    return 0;
}

void writeAnswer(char *outputFile, int n, const double *X, int result) {
    int i;
    FILE *output = fopen(outputFile, "w");
    if (result == -1) {
        fprintf(output, "%d\n", 0);
    } else {
        fprintf(output, "%d\n", n);
        for (i = 0; i < n; i++) {
            fprintf(output, "%1.9lf\n", *(X + i));
        }
    }
}

void printHelp() {
    printf(
        "Usage: lss [input_file_name] [output_file_name] [options]\n"
        "Where options include:\n"
        " -d                print debug messages [default OFF]\n"
        " -e                print errors [default OFF]\n"
        " -p                print matrix [default OFF]\n"
        " -t                print execution time [default OFF]\n"
        " -h, -?            print this and exit\n");
}

void print(int n, double *A) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%1.9lf ", *(A + i * n + j));
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int n, setInput = 0;
    double *A, *B, *X, *tmp;
    char *inputFile = "lss_71_01_in.txt";
    char *outputFile = "lss_71_01_out.txt";

    switch (validateParams(argc, argv)) {
        case 1: {
            if (flag_e) {
                printf("ValidationError: Wrong syntax of parameters. Too much filenames\n");
            }
            return 11;
        }
        case 2: {
            if (flag_e) {
                printf(
                    "ValidationError. Wrong syntax of parameters. There is no such parameter or "
                    "you haven't set value to one of the parameters\n");
            }
            return 2;
        }
        default: {
            break;
        }
    }

    if (flag_q || flag_h) {
        printHelp();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!setInput) {
                if (i != 1) {
                    if (flag_e) {
                        printf("ValidationError: Wrong order of parameters.\n");
                    }
                    return 4;
                }
                inputFile = argv[i];
                if (!validateFile(inputFile)) {
                    if (flag_e) {
                        printf("ValidationError: There is no such file.\n");
                    }
                    return 5;
                }
                setInput = 1;
            } else {
                if (i != 2) {
                    if (flag_e) {
                        printf("ValidationError: Wrong order of parameters.\n");
                    }
                    return 4;
                }
                outputFile = argv[i];
            }
        }
    }

    if (!setInput) {
        if (!validateFile(inputFile)) {
            if (flag_e) {
                printf("ValidationError: There is no such file.\n");
            }
            return 5;
        }
    }

    switch (readInput(inputFile, &A, &B, &n)) {
        case 6: {
            if (flag_e) {
                printf("ValidationError. File is empty.\n");
            }
            return 6;
        }
        case 7: {
            if (flag_e) {
                printf("ValidationError. n is not a positive integer.\n");
            }
            return 7;
        }
        case 8: {
            if (flag_e) {
                printf("ValidationError. Not enough elements in the matrix.\n");
            }
            return 8;
        }
        case 9: {
            if (flag_e) {
                printf("ValidationError. One of the elements of the matrix is not a number.\n");
            }
            return 9;
        }
        default: {
            break;
        }
    }

    tmp = malloc(lss_memsize_71_01(n));
    X = malloc(n * sizeof(double));

    clock_t begin = clock();

    if (flag_p) {
        print(n, A);
    }

    lss_71_01(n, A, B, X, tmp);
    clock_t end = clock();

    if (flag_p) {
        print(n, A);
    }

    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    if (flag_t) {
        printf("Execution time: %1.9lf\n", timeSpent);
    }

    // writeAnswer(outputFile, n, X, 1);

    // return result;

    free(tmp);
    free(X);
    return 0;
}
