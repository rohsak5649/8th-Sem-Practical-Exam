#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;
long parallelSum(const vector<int>& arr) {
    long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}
int parallelMax(const vector<int>& arr) {
    int max_val = arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}
int parallelMin(const vector<int>& arr) {
    int min_val = arr[0];
    #pragma omp parallel for reduction(min:min_val)
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    return min_val;
}
double parallelAverage(const vector<int>& arr) {
    long sum = parallelSum(arr); 
    return static_cast<double>(sum) / arr.size();
}
int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    long sum = parallelSum(arr);
    int max_val = parallelMax(arr);
    int min_val = parallelMin(arr);
    double average = parallelAverage(arr);
    cout << "Sum: " << sum << endl;
    cout << "Max: " << max_val << endl;
    cout << "Min: " << min_val << endl;
    cout << "Average: " << average << endl;
    return 0;
}
