#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

// Sequential Bubble Sort
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);
}

// Parallel Bubble Sort
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    while (swapped) {
        swapped = false;
        #pragma omp parallel for shared(arr)
        for (int i = 0; i < n - 1; i += 2) { // Even phase
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }

        #pragma omp parallel for shared(arr)
        for (int i = 1; i < n - 1; i += 2) { // Odd phase
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

// Merge function for both versions
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= m) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = l; k <= r; k++)
        arr[k] = temp[k - l];
}

// Sequential Merge Sort
void mergeSortSequential(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void mergeSortParallel(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, l, m);
            #pragma omp section
            mergeSortParallel(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> input(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> input[i];

    vector<int> arr;

    // Bubble Sort Sequential
    arr = input;
    clock_t t1 = clock();
    bubbleSortSequential(arr);
    clock_t t2 = clock();
    cout << "Sequential Bubble Sort Time: " << double(t2 - t1) / CLOCKS_PER_SEC << " sec" << endl;

    // Bubble Sort Parallel
    arr = input;
    clock_t t3 = clock();
    bubbleSortParallel(arr);
    clock_t t4 = clock();
    cout << "Parallel Bubble Sort Time: " << double(t4 - t3) / CLOCKS_PER_SEC << " sec" << endl;

    // Merge Sort Sequential
    arr = input;
    clock_t t5 = clock();
    mergeSortSequential(arr, 0, n - 1);
    clock_t t6 = clock();
    cout << "Sequential Merge Sort Time: " << double(t6 - t5) / CLOCKS_PER_SEC << " sec" << endl;

    // Merge Sort Parallel
    arr = input;
    clock_t t7 = clock();
    mergeSortParallel(arr, 0, n - 1);
    clock_t t8 = clock();
    cout << "Parallel Merge Sort Time: " << double(t8 - t7) / CLOCKS_PER_SEC << " sec" << endl;

    return 0;
}
