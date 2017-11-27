

#include <stdio.h>


int swap(int* num1, int* num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low-1;
    int j;
    for(j=low; j<=high-1; j++) {
        if (arr[j] <= pivot){
            i++;
            swap(&arr[j], &arr[i]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return (i+1);
}

void quicksort(int arr[], int low, int high) {
    if (low < high){
        int p = partition(arr, low, high);

        quicksort(arr, low, p-1);
        quicksort(arr, p+1, high);
    }
}


int main(){

    int array[] = {5, 71, 45, 13, 11, 4, 6};
    quicksort(array, 0, 6);
    int i;
    for(i = 0; i < 7; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
