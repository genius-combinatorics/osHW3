#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *arr;          // Dynamic array
int *sorted_arr;  
int SIZE;          // Will be determined by user input
int mid;           // Calculated after SIZE is known

// Bubble sort for a subarray
void bubble_sort(int start, int end) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - 1 - (i - start); j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Thread to sort the left half
void* sort_left(void* arg) {
    bubble_sort(0, mid);
    pthread_exit(NULL);
}

// Thread to sort the right half
void* sort_right(void* arg) {
    bubble_sort(mid, SIZE);
    pthread_exit(NULL);
}

// Thread to merge the two sorted halves
void* merge(void* arg) {
    int i = 0, j = mid, k = 0;
    while (i < mid && j < SIZE) {
        if (arr[i] < arr[j]) {
            sorted_arr[k++] = arr[i++];
        } else {
            sorted_arr[k++] = arr[j++];
        }
    }
    while (i < mid) sorted_arr[k++] = arr[i++];
    while (j < SIZE) sorted_arr[k++] = arr[j++];
    pthread_exit(NULL);
}

int main() {
    // Get array size from user
    printf("Enter the size of the array: ");
    scanf("%d", &SIZE);
    
    // Validate input size
    if (SIZE <= 0) {
        printf("Invalid array size. Please enter a positive integer.\n");
        return 1;
    }
    
    // Allocate memory for arrays
    arr = (int*)malloc(SIZE * sizeof(int));
    sorted_arr = (int*)malloc(SIZE * sizeof(int));
    mid = SIZE / 2;
    
    // Get array elements from user
    printf("Enter %d integers (separated by spaces or newlines):\n", SIZE);
    for (int i = 0; i < SIZE; i++) {
        scanf("%d", &arr[i]);
    }

    // Thread execution
    pthread_t tid1, tid2, tid_merge;

    // PHASE 1: Sort in parallel
    pthread_create(&tid1, NULL, sort_left, NULL);
    pthread_create(&tid2, NULL, sort_right, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // PHASE 2: Merge
    pthread_create(&tid_merge, NULL, merge, NULL);
    pthread_join(tid_merge, NULL);

    // PHASE 3: Print result
    printf("Sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    // Free allocated memory
    free(arr);
    free(sorted_arr);

    return 0;
}
