#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& arr, int left, int right)
{
    int pivot = arr[left];
    while(left < right)
    {
        while(left < right && arr[right] >= pivot)
            right--;
        arr[left] = arr[right];
        while(left < right && arr[left] <= pivot)
            left++;
        arr[right] = arr[left];
    }

    arr[left] = pivot;
    return left;
}

void qsort(vector<int>& arr, int left, int right)
{
    if(left > right)
        return;
    int m = partition(arr, left, right);
    qsort(arr, left, m - 1);
    qsort(arr, m + 1, right);
}

void print(vector<int>& arr) {
    for(int ai : arr) {
        cout << ai << " ";
    }
    cout << endl;
}

int main() {
    vector<int> a = {13, 21, 4, 7, 10, 1, 32, 18, 22};
    print(a);
    qsort(a, 0, a.size() - 1);
    print(a);
    return 0;
}