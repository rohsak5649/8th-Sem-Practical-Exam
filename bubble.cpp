#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    vector<future<void>> futures;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            futures.push_back(async(launch::async, [&arr, j]() {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }));
        }
        for (auto& f : futures) {
            f.get();
        }
        futures.clear();
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> leftArray(n1), rightArray(n2);
    for (int i = 0; i < n1; i++) leftArray[i] = arr[left + i];
    for (int j = 0; j < n2; j++) rightArray[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            arr[k] = leftArray[i];
            i++;
        } else {
            arr[k] = rightArray[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        future<void> leftFuture = async(launch::async, [&]() { parallelMergeSort(arr, left, mid); });
        future<void> rightFuture = async(launch::async, [&]() { parallelMergeSort(arr, mid + 1, right); });
        leftFuture.get();
        rightFuture.get();
        merge(arr, left, mid, right);
    }
}

long long measureTime(function<void()> sortMethod) {
    auto start = chrono::high_resolution_clock::now();
    sortMethod();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    return duration.count();
}

void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int choice;
    bool continueRunning = true;

    while (continueRunning) {
        cout << "\nChoose an option:" << endl;
        cout << "1. Sequential Bubble Sort" << endl;
        cout << "2. Parallel Bubble Sort" << endl;
        cout << "3. Sequential Merge Sort" << endl;
        cout << "4. Parallel Merge Sort" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 5) {
            continueRunning = false;
            cout << "Exited the program successfully" << endl;
            break;
        }

        int size;
        cout << "Enter the number of elements: ";
        cin >> size;

        vector<int> arr(size);
        cout << "Enter the " << size << " space-separated values of the array: ";
        for (int i = 0; i < size; i++) {
            cin >> arr[i];
        }

        switch (choice) {
            case 1: {
                cout << "\nPerforming Sequential Bubble Sort..." << endl;
                vector<int> arrBubbleSeq = arr;
                long long bubbleSeqTime = measureTime([&]() { bubbleSort(arrBubbleSeq); });
                cout << "Sorted Array: ";
                printArray(arrBubbleSeq);
                cout << "Time taken: " << bubbleSeqTime << " ms" << endl;
                break;
            }
            case 2: {
                cout << "\nPerforming Parallel Bubble Sort..." << endl;
                vector<int> arrBubblePar = arr;
                long long bubbleParTime = measureTime([&]() { parallelBubbleSort(arrBubblePar); });
                cout << "Sorted Array: ";
                printArray(arrBubblePar);
                cout << "Time taken: " << bubbleParTime << " ms" << endl;
                break;
            }
            case 3: {
                cout << "\nPerforming Sequential Merge Sort..." << endl;
                vector<int> arrMergeSeq = arr;
                long long mergeSeqTime = measureTime([&]() { mergeSort(arrMergeSeq, 0, arrMergeSeq.size() - 1); });
                cout << "Sorted Array: ";
                printArray(arrMergeSeq);
                cout << "Time taken: " << mergeSeqTime << " ms" << endl;
                break;
            }
            case 4: {
                cout << "\nPerforming Parallel Merge Sort..." << endl;
                vector<int> arrMergePar = arr;
                long long mergeParTime = measureTime([&]() { parallelMergeSort(arrMergePar, 0, arrMergePar.size() - 1); });
                cout << "Sorted Array: ";
                printArray(arrMergePar);
                cout << "Time taken: " << mergeParTime << " ms" << endl;
                break;
            }
            default:
                cout << "Invalid choice! Please choose a valid option." << endl;
                break;
        }

        if (continueRunning) {
            cout << "\nDo you want to continue? (yes/no): ";
            string userChoice;
            cin >> userChoice;
            if (userChoice == "no" || userChoice == "No") {
                continueRunning = false;
                cout << "Exited the program successfully" << endl;
            }
        }
    }

    return 0;
}
