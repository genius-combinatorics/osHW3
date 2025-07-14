#include <stdio.h>
#include <pthread.h>

#define SIZE 10

int arr[SIZE] = {38, 27, 43, 3, 9, 82, 10, 65, 1, 56}; 
int sorted_arr[SIZE];  
int mid = SIZE / 2;

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
    while (i < mid) sorted_arr[k++] = arr[i++];  // Remaining left elements
    while (j < SIZE) sorted_arr[k++] = arr[j++]; // Remaining right elements
    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2, tid_merge;

    // --- PHASE 1: Sort in parallel ---
    pthread_create(&tid1, NULL, sort_left, NULL);  // Thread 1: Left half
    pthread_create(&tid2, NULL, sort_right, NULL);  // Thread 2: Right half

    // Wait for sorting threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // --- PHASE 2: Merge ---
    pthread_create(&tid_merge, NULL, merge, NULL);  // Thread 3: Merge
    pthread_join(tid_merge, NULL);                  // Wait for merge to finish

    // --- PHASE 3: Parent thread prints the result ---
    printf("Parent thread prints the sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    return 0;
}
