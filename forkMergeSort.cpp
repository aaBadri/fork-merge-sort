#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

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

/*void sort(int a[],int startIndex,int endIndex){
    int i,j;
    int n =
    for(i=1;i<n;++i)
    {
        for(j=0;j<(n-i);++j)
            if(a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
    }
}*/
void merge_sort(int arr[], int startIndex, int endIndex) {
    int midIndex;
    int state;
    pid_t leftChild = 0;
    pid_t rightChild = 0;


    if (endIndex - startIndex <= 1)
        return;

    midIndex = (startIndex + endIndex) / 2;
    leftChild = fork();
    if (leftChild < 0) {
        perror("fork failed");
        exit(1);
    } else if (leftChild == 0) {
         cout<<"left child : "<<startIndex<<" , "<<midIndex<<endl;
        merge_sort(arr, startIndex, midIndex);
        exit(1);
    } else if (leftChild > 0) {
        rightChild = fork();
        if (rightChild < 0) {
            perror("fork failed");
            exit(1);
        } else if (rightChild == 0) {
            cout<<"right child : "<<midIndex + 1<<" , "<<endIndex<<endl;
            merge_sort(arr, midIndex + 1, endIndex);
            exit(1);
        }
    }
    // else if (rightChild > 0) {
    waitpid(leftChild, &state, 0);
    waitpid(rightChild, &state, 0);
    cout<<"merge : "<<startIndex<<" , "<<endIndex<<endl;
    merge(arr, startIndex, endIndex);
    for (int i = startIndex; i <= endIndex; ++i) {
        cout<<arr[i]<<endl;
    }
    //}

}

int main(int argc, char *argv[]) {

    int shmid;
    const int length = 10;
    int *shmarray;
    shmid = shmget(IPC_PRIVATE, length * sizeof(int), IPC_CREAT | 0666);

    if (shmid < 0) {
        fprintf(stderr, "failed to create shm segment\n");
        perror("shmget");
        exit(1);
    }
    shmarray = (int *) shmat(shmid, NULL, 0);
    if (*shmarray == -1)
        return -1;

    for (int i = length; i >= 0; i--) {
        shmarray[length - i] = i;
    }
    //for (int i = 0; i < length; i++)
      //  cout << shmarray[i] << endl;
    merge_sort(shmarray, 0, length - 1);
   // for (int i = 0; i < length; i++)
     //   cout << shmarray[i] << endl;
    return 0;
}
