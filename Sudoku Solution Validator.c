#include <stdio.h>
#include <pthread.h>

#define N 9  

int sudoku[N][N] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

int valid = 1;  // متغیر برای ذخیره وضعیت کلی جدول معتبر بودن

void* check_row(void* arg) {
    int row = *((int*) arg);
    int seen[N+1] = {0};
    for (int col = 0; col < N; col++) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || seen[num]) {
            valid = 0;
            pthread_exit(NULL);
        }
        seen[num] = 1;
    }
    pthread_exit(NULL);
}

void* check_col(void* arg) {
    int col = *((int*) arg);
    int seen[N+1] = {0};
    for (int row = 0; row < N; row++) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || seen[num]) {
            valid = 0;
            pthread_exit(NULL);
        }
        seen[num] = 1;
    }
    pthread_exit(NULL);
}

void* check_box(void* arg) {
    int box_index = *((int*) arg);
    int seen[N+1] = {0};
    int start_row = (box_index / 3) * 3;
    int start_col = (box_index % 3) * 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = sudoku[start_row + i][start_col + j];
            if (num < 1 || num > 9 || seen[num]) {
                valid = 0;
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[27];
    int index[27];

    for (int i = 0; i < N; i++) {
        index[i] = i;
        pthread_create(&threads[i], NULL, check_row, &index[i]);
    }

    for (int i = 0; i < N; i++) {
        index[i + N] = i;
        pthread_create(&threads[i + N], NULL, check_col, &index[i + N]);
    }

    for (int i = 0; i < N; i++) {
        index[i + 2 * N] = i;
        pthread_create(&threads[i + 2 * N], NULL, check_box, &index[i + 2 * N]);
    }

    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    if (valid)
        printf("Sudoku is valid.\n");
    else
        printf("Sudoku is NOT valid.\n");

    return 0;
}

