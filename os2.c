#include <stdio.h>
#include <pthread.h>

#define SIZE 10  // اندازه آرایه (قابل تغییر)

int arr[SIZE] = {38, 27, 43, 3, 9, 82, 10, 65, 1, 56};  // آرایه اصلی
int sorted_arr[SIZE];  // آرایه نهایی مرتب شده

int mid = SIZE / 2;

// تابع مرتب سازی حبابی روی بخشی از آرایه
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

// ترد مرتب سازی نیمه اول
void* sort_left(void* arg) {
    bubble_sort(0, mid);
    pthread_exit(NULL);
}

// ترد مرتب سازی نیمه دوم
void* sort_right(void* arg) {
    bubble_sort(mid, SIZE);
    pthread_exit(NULL);
}

// ترد ادغام دو نیمه مرتب شده
void* merge(void* arg) {
    int i = 0;       // اندیس نیمه اول
    int j = mid;     // اندیس نیمه دوم
    int k = 0;       // اندیس آرایه نهایی

    while (i < mid && j < SIZE) {
        if (arr[i] < arr[j]) {
            sorted_arr[k++] = arr[i++];
        } else {
            sorted_arr[k++] = arr[j++];
        }
    }
    // اگر عناصر نیمه اول باقی ماند
    while (i < mid) {
        sorted_arr[k++] = arr[i++];
    }
    // اگر عناصر نیمه دوم باقی ماند
    while (j < SIZE) {
        sorted_arr[k++] = arr[j++];
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2, tid_merge;

    // ایجاد دو ترد مرتب سازی
    pthread_create(&tid1, NULL, sort_left, NULL);
    pthread_create(&tid2, NULL, sort_right, NULL);

    // منتظر پایان تردهای مرتب سازی
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // ایجاد ترد ادغام
    pthread_create(&tid_merge, NULL, merge, NULL);

    // منتظر پایان ترد ادغام
    pthread_join(tid_merge, NULL);

    // چاپ آرایه مرتب شده نهایی
    printf("Sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    return 0;
}
