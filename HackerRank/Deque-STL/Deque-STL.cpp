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

void printKMax(vector<int> arr, int n, int k) {
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

int main() {

    printKMax({3,4,6,3,4}, 5, 2);
    printKMax({ 3 ,4, 5, 8, 1 ,4 ,10 }, 7, 4);
    return 0;
}
