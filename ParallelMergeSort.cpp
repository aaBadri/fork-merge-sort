#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;
const int BASE = 10;
const int LENGTH = 100;

/*
 * a parallel merge sort using fork and shared memory
 */
void merge(int arr[], int startIndex, int mid, int endIndex) {
    int firstIndex, secIndex, mainIndex;

    int leftSize = mid - startIndex + 1;
    int rightSize = endIndex - mid;

    int leftArr[leftSize], rightArr[rightSize];

    for (firstIndex = 0; firstIndex < leftSize; firstIndex++)
        leftArr[firstIndex] = arr[startIndex + firstIndex];
    for (secIndex = 0; secIndex < rightSize; secIndex++)
        rightArr[secIndex] = arr[mid + 1 + secIndex];

    firstIndex = secIndex = 0;
    mainIndex = startIndex;
    while (firstIndex < leftSize && secIndex < rightSize) {
        if (leftArr[firstIndex] <= rightArr[secIndex]) {
            arr[mainIndex] = leftArr[firstIndex];
            firstIndex++;
        } else {
            arr[mainIndex] = rightArr[secIndex];
            secIndex++;
        }
        mainIndex++;
    }

    while (firstIndex < leftSize) {
        arr[mainIndex] = leftArr[firstIndex];
        firstIndex++;
        mainIndex++;
    }
    while (secIndex < rightSize) {
        arr[mainIndex] = rightArr[secIndex];
        secIndex++;
        mainIndex++;
    }
}

void mergeSort(int arr[], int startIndex, int endIndex) {
    int state;
    pid_t leftChild = 0;
    pid_t rightChild = 0;

    int mid = (startIndex + startIndex) / 2;
    if (endIndex - startIndex <= 0) {
        return;
    }

    else if (endIndex - startIndex <= BASE) {
        mergeSort(arr, startIndex, mid);
        mergeSort(arr, mid + 1, endIndex);
    }

    leftChild = fork();
    if (leftChild < 0) {
        perror("fork failed");
        exit(1);
    } else if (leftChild == 0) {
        mergeSort(arr, startIndex, mid);
        exit(1);
    } else if (leftChild > 0) {
        rightChild = fork();
        if (rightChild < 0) {
            perror("fork failed");
            exit(1);
        } else if (rightChild == 0) {
            mergeSort(arr, mid + 1, endIndex);
            exit(1);
        }
    }
    waitpid(leftChild, &state, 0);
    waitpid(rightChild, &state, 0);
    merge(arr, startIndex, mid, endIndex);
}

int main(int argc, char *argv[]) {
    int arr_id;
    int *arr;

    arr_id = shmget(IPC_PRIVATE, LENGTH * sizeof(int), IPC_CREAT | 0666);

    if (arr_id < 0) {
        perror("shmget failed");
        exit(1);
    }

    arr = (int *) shmat(arr_id, NULL, 0);
    if (*arr == -1)
        return -1;

    for (int i = 0; i < LENGTH; i++) {
        arr[i] = rand() % (4 * LENGTH);
    }

    mergeSort(arr, 0, LENGTH - 1);

    for (int i = 0; i < LENGTH; i++)
        cout << arr[i] << endl;
    return 0;
}