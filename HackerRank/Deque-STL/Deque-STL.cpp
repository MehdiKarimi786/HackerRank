#include <iostream>
#include <algorithm>
#include <deque> 
#include <map>
#include <vector>
using namespace std;

void printKMax1(vector<int> arr, int n, int k) {
    //Write your code here.
    map<int, int> cur;
    for (auto i(0); i < n; ++i)
    {
        auto val = arr[i];
        auto it = cur.find(val);
        if (it == cur.end())
        {
            cur[val] = 1;
        }
        else
        {
            ++cur[val];
        }

        if (i + 1 >= k)
        {
            cout << cur.rbegin()->first << " ";
            auto firstVal = arr[i + 1 - k];
            --cur[firstVal];
            if (cur[firstVal] == 0)
            {
                cur.erase(firstVal);
            }
        }
    }
    cout << endl;
}

void printKMax2(vector<int> arr, int n, int k) {
    //Write your code here.
    map<int, int> cur;
    for (auto i(0); i < n; ++i)
    {
        auto val = arr[i];
        auto it = cur.find(val);
        if (it == cur.end())
        {
            cur.insert({ val, 1 });
        }
        else
        {
            ++cur[val];
        }

        if (i + 1 >= k)
        {
            auto it = find_if(cur.rbegin(), cur.rend(), [](const auto &pr) {
                return pr.second > 0;
                });
            cout << it->first << " ";
            auto firstVal = arr[i + 1 - k];
            --cur[firstVal];
        }
    }
    cout << endl;
}

void printKMax(int arr[], int n, int k) {
    auto maxVal = *max_element(arr, arr + k);
    cout << maxVal << " ";
    for (auto i(k); i < n ; ++i)
    {
        auto &val = arr[i];
        if (val == maxVal)
        {
            // Do nothing
        }
        else if(val > maxVal)
        {
            maxVal = val;
        }
        else if(maxVal != arr[i - k])
        {
            // Do nothing
        }
        else
        {
            maxVal = *max_element(arr + i - k + 1, arr + i + 1);
        }

        cout << maxVal << " ";
    }
    cout << endl;
}

int main() {
    int arr1[] = { 3,4,6,3,4 };
    printKMax(arr1, 5, 2);

    int arr2[] = { 3 ,4, 5, 8, 1 ,4 ,10 };
    printKMax(arr2, 7, 4);
    return 0;
}
