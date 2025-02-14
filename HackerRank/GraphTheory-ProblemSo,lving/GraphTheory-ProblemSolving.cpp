// GraphTheory-ProblemSo,lving.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int problemSolving(int k, vector<int> v) 
{
    auto result = 0;
    auto sz = v.size();

    for (size_t i = 0; i < sz; ++i)
    {
        auto vCur = v[i];
        if(vCur <= 0) continue;
        
        v[i] = -v[i];
        ++result;
        
        for (size_t j = i + 1; j < sz; ++j)
        {
            auto vNxt = v[j];
            if(vNxt <= 0) continue;
            if (vNxt >= k + vCur || vCur >= k + vNxt)
            {
                vCur = vNxt;
                v[j] = -v[j];
            }
        }
    }

    return result;
}

vector<pair<vector<int>, int>> tests = {
    {{5,4,7},2},
    {{5,3,4,5,6},1},
};

int main()
{
    ifstream file("C:/Users/aei7fe/source/HackerRank/HackerRank/GraphTheory-ProblemSo,lving/tests.txt");
    
    int testCounts;
    file >> testCounts;
    while (testCounts > 0)
    {
        int vCounts;
        int k;
        file >> vCounts >> k;
        vector<int> v(vCounts, 0);
        int idx = 0;
        while (vCounts > 0)
        {
            file >> v[idx];
            --vCounts;
            ++idx;
        }
        tests.push_back(std::make_pair(v, k));
        --testCounts;
    }

    for(auto test: tests)
    {
        cout << problemSolving(test.second, test.first) << endl;
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
