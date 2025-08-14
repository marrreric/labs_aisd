#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>

int MINRUN = 32;

void insertionSort(int* arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int minRunLength(int n) {
    int r = 0;
    while (n >= MINRUN) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}


void merge(int* arr, int left, int mid, int right) {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    int* leftArr = new int[len1];
    int* rightArr = new int[len2];

    for (int i = 0; i < len1; i++)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < len2; i++)
        rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    
    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        }
        else {
            int gallopCount = 0;
            while (j + gallopCount < len2 && rightArr[j + gallopCount] < leftArr[i]) {
                gallopCount++;
            }
            for (int g = 0; g < gallopCount; g++) {
                arr[k++] = rightArr[j++];
            }
        }
    }

    while (i < len1) {
        arr[k++] = leftArr[i++];
    }

    while (j < len2) {
        arr[k++] = rightArr[j++];
    }

    delete[] leftArr;
    delete[] rightArr;
}

std::vector<std::pair<int, int>> findRuns(int* arr, int n) {
    std::vector<std::pair<int, int>> runs;
    int start = 0;

    while (start < n) {
        int end = start;
        while (end < n - 1 && arr[end] <= arr[end + 1]) {
            end++;
        }
        if (end < n - 1) {
            while (end < n - 1 && arr[end] >= arr[end + 1]) {
                end++;
            }
            std::reverse(arr + start, arr + end + 1);
        }
        runs.emplace_back(start, end);
        start = end + 1;
    }

    return runs;
}


void timSort(int* arr, int n) {
    std::stack<std::pair<int, int>> stk;
    MINRUN = minRunLength(n);

    auto runs = findRuns(arr, n);

    for (const auto& run : runs) {
        insertionSort(arr, run.first, run.second);
        stk.emplace(run.first, run.second);
    }

    while (stk.size() > 1) {
        auto second = stk.top(); stk.pop();
        auto first = stk.top(); stk.pop();

        if (stk.empty() || second.second >= first.second) {
            merge(arr, first.first, first.second, second.second);
            stk.push({ first.first, second.second });
        }
        else {
            stk.push(first);
            stk.push(second);
        }
    }
}

void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
void printMenu() {
    std::cout << "Введите количество элементов массива (максимум 100), или 0 для выхода:" << std::endl;
}

int main() {
    setlocale(LC_ALL, "rus");

    while (true) {
        int n;
        printMenu();
        std::cin >> n;

        if (n == 0) {
            break; 
        }

        if (n > 30000 || n <= 0) {
            std::cout << "Неправильный ввод. Пожалуйста, введите число от 1 до 100." << std::endl;
            continue; 
        }

        int* arr = new int[n];
        std::srand(std::time(0));

        for (int i = 0; i < n; i++) {
            arr[i] = std::rand() % 1000 + 1;
        }

        std::cout << "Оригинальный массив: ";
        printArray(arr, n);

        timSort(arr, n);

        std::cout << "Отсортированный массив: ";
        printArray(arr, n);

        delete[] arr;

        
        std::cout << "Нажмите Enter для продолжения...";
        std::cin.ignore();
        std::cin.get();
        system("cls"); 
    }

    return 0;
}
