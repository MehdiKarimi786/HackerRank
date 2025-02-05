#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <set>
using namespace std;


int main()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    unsigned int n=4;
    //std::cin >> n;
    if (n > 2)
    {
        std::set<std::tuple<unsigned int, unsigned  int, unsigned int>> result;
        std::set<int> used;
        for (unsigned int i = 0; i < n; ++i)
        {
            for (unsigned int j = i + 1; (j + i) < n; ++j)
            {
                auto diff = n - (i + j);
                auto end_it = used.end();
                if (diff > j && used.find(i) == end_it && used.find(j) == end_it && used.find(diff) == end_it)
                {
                    used.insert(i);
                    used.insert(j);
                    used.insert(diff);
                    result.insert(std::make_tuple(i, j, diff));
                }
            }
        }

        printf("%d\n", result.size() * 3);
        for (auto tp : result)
        {
            auto x = std::get<0>(tp);
            auto y = std::get<1>(tp);
            auto z = std::get<2>(tp);
            printf("%d %d %d\n", x, z, y);
            printf("%d %d %d\n", y, x, z);
            printf("%d %d %d\n", z, y, x);
        }
        printf("\n");
    }

    return 0;
}
