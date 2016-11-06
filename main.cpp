#include <iostream>

using namespace std;

void merge(int a[], int startIndex, int endIndex) {

    int arrSize = (endIndex - startIndex) + 1;
    int *temp = new int[arrSize]();

    int firstIndex = startIndex;
    int mid = (startIndex + endIndex) / 2;
    int mainIndex = 0;
    int secIndex = mid + 1;

    while (mainIndex < arrSize) {
        if ((firstIndex <= mid) && (a[firstIndex] < a[secIndex])) {
            temp[mainIndex++] = a[firstIndex++];
        } else {
            temp[mainIndex++] = a[secIndex++];
        }

    }
    for (mainIndex = 0; mainIndex < arrSize; mainIndex++) {
        a[startIndex + mainIndex] = temp[mainIndex];
    }
    delete[]temp;

}

void merge_sort(int arr[], int startIndex, int endIndex) {
    int midIndex;
    if (startIndex >= endIndex)
        return;
    midIndex = (startIndex + endIndex) / 2;
    merge_sort(arr, startIndex, midIndex);
    merge_sort(arr, midIndex + 1, endIndex);
    merge(arr, startIndex, endIndex);
}

int main(int argc, char *argv[]) {
    const int length = 10;
    int arr[10] = {2, 5, 6, 4, 7, 2, 8, 3, 9, 10};
    merge_sort(arr, 0, length - 1);
    for (int i = 0; i < length; i++) {
        cout << arr[i] << endl;
    }
    return 0;
}
