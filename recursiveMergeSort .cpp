#include <iostream>

using namespace std;

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
    if (startIndex >= endIndex) {
        return;
    }
        int mid = (startIndex + startIndex) / 2;

        mergeSort(arr, startIndex, mid);
        mergeSort(arr, mid + 1, endIndex);

        merge(arr, startIndex, mid, endIndex);

}

int main(int argc, char *argv[]) {
    const int length = 10;
    int arr[length];
    for (int i = length; i >= 0; i--) {
        arr[length - i] = rand()%(3*length);
    }
    mergeSort(arr, 0, length - 1);
    for (int i = 0; i < length; i++)
        cout << arr[i] << endl;
    return 0;
}