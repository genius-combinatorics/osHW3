#include <stdio.h>
#include <pthread.h>

#define N 9  

int sudoku[N][N];
int valid = 1;  // Variable to store the overall validity status

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

void read_sudoku() {
    printf("Enter the Sudoku puzzle (9x9 matrix, numbers separated by spaces):\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &sudoku[i][j]);
        }
    }
}

int main() {
    pthread_t threads[27];
    int index[27];

    // Read the Sudoku puzzle from user input
    read_sudoku();

    // Create threads for row checks
    for (int i = 0; i < N; i++) {
        index[i] = i;
        pthread_create(&threads[i], NULL, check_row, &index[i]);
    }

    // Create threads for column checks
    for (int i = 0; i < N; i++) {
        index[i + N] = i;
        pthread_create(&threads[i + N], NULL, check_col, &index[i + N]);
    }

    // Create threads for box checks
    for (int i = 0; i < N; i++) {
        index[i + 2 * N] = i;
        pthread_create(&threads[i + 2 * N], NULL, check_box, &index[i + 2 * N]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result
    if (valid)
        printf("Sudoku is valid.\n");
    else
        printf("Sudoku is NOT valid.\n");

    return 0;
}
