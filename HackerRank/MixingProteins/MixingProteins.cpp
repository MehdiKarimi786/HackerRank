#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

/*
 * Complete the 'pmix' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts following parameters:
 *  1. STRING s
 *  2. INTEGER k
 */

string pmix1(string s, int k) {
    static map<string, char> ring2 = {
        {"AA", 'A'},
        {"AB", 'B'},
        {"AC", 'C'},
        {"AD", 'D'},
        {"BA", 'B'},
        {"BB", 'A'},
        {"BC", 'D'},
        {"BD", 'C'},
        {"CA", 'C'},
        {"CB", 'D'},
        {"CC", 'A'},
        {"CD", 'B'},
        {"DA", 'D'},
        {"DB", 'C'},
        {"DC", 'B'},
        {"DD", 'A'},
    };
    int sz = s.size();
    for (auto i(0); i < k; ++i)
    {
        auto firstCh = s[0];
        auto lastCh = s[sz -1];
        for (auto ch(0); ch < sz - 1; ++ch)
        {
            string v({s[ch], s[ch+1] });
            s[ch] = ring2[v];
        }
        string v({ lastCh, firstCh });
        s[sz -1] = ring2[v];
    }
    return s;
}

string pmix(string s, int k) {
    static vector<vector<char>> ring2 = {
        {'A', 'B', 'C', 'D'},
        {'B', 'A', 'D', 'C'},
        {'C', 'D', 'A', 'B'},
        {'D', 'C', 'B', 'A'},
    };
    int sz = s.size();
    auto data = s.data();
    for (auto i(0); i < k; ++i)
    {
        auto firstCh = data[0];
        auto lastCh = data[sz - 1];
        for (auto ch(0); ch < sz - 1; ++ch)
        {
            data[ch] = ring2[data[ch] - 'A'][data[ch + 1] - 'A'];
        }
        data[sz - 1] = ring2[lastCh - 'A'][firstCh - 'A'];
    }
    return s;
}


int main()
{
    cout << pmix("AAAAD", 15) << endl;

    ifstream file("C:/Users/aei7fe/source/HackerRank/HackerRank/MixingProteins/tests.txt");
    int n, k;
    file >> n >> k;
    string s;
    file >> s;
    cout << pmix(s, k) << endl;
    return 0;

}
