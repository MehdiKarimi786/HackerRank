#include <tuple>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

using Command = vector<int>;
using Commands = vector<Command>;
using Knights = vector<int>;
auto tests = vector<tuple<int, Commands , Knights >>{
    {7, {{1,2,4},{2,3,3},{3,4,1},{3,4,0},}, {0, 6, 9, 11, 24,25, 48}},
};


void kingRichardKnights(int n, vector<vector<int>> &commands, vector<int> &knights, vector<pair<int,int>> & result)
{
    struct Rect {
        inline void set(int top, int left, int length) noexcept
        {
            top_ = top;
            bottom_ = top + length;
            left_ = left;
            right_ = left + length;
            length_ = length;
        }
        
        inline bool isRect(int posRow, int posCol) const noexcept
        {
            return (top_ <= posRow && posRow <= bottom_) &&
                (left_ <= posCol && posCol <= right_);
        }

        inline void rotate(int &posRow, int &posCol, int knight) const noexcept
        {
            auto row = posRow;
            auto col = posCol;
            auto offset = std::min({ row - top_ , bottom_ - row, col - left_, right_ - col });
            if (col == left_ + offset)
            {
                auto delta = row - top_;
                posCol = right_ - delta;
                posRow = top_ + offset;
            }
            else if (col == right_ - offset)
            {
                auto delta = row - top_;
                posCol = right_ - delta;
                posRow = bottom_ - offset;
            }
            else if (row == top_ + offset)
            {
                auto delta = col - left_;
                posCol = right_ - offset;
                posRow = top_ + delta;
            }
            else
            {
                auto delta = col - left_;
                posCol = left_ + offset;
                posRow = bottom_ - delta;
            }
        }

        int top_ = -1;
        int bottom_ = -1;
        int left_ = -1;
        int right_ = -1;
        int length_ = 0;
    };

    auto found = false;
    do {
        found = false;
        Commands uniqueCommands;
        auto sz = commands.size();
        for (auto cmd(0u); cmd < sz;)
        {
            auto cur = commands[cmd];
            auto count = 1;
            for (auto cmdNx(cmd + 1); cmdNx < sz; ++cmdNx)
            {
                auto& curNx = commands[cmdNx];
                if (cur == curNx) ++count;
                else break;
            }

            if (count < 4)
            {
                ++cmd;
                cur.push_back(count);
                uniqueCommands.push_back(cur);
            }
            else
            {
                found = true;
                count /= 4;
                count *= 4;
                cmd += count;
            }
        }
        commands = uniqueCommands;
    } while (found);

    Rect rect;
    int resultIdx = 0;
    for (auto knight : knights)
    {
        auto posRow = knight / n;
        auto posCol = knight% n;
        for (auto command : commands)
        {
            auto top = command[0] - 1;
            auto left = command[1] - 1;
            auto length = command[2];
            auto rotateCount = command[3];
            rect.set(top, left, length);
            if (rect.isRect(posRow, posCol))
            {
                for (auto r(0); r < rotateCount; ++r)
                {
                    rect.rotate(posRow, posCol, knight);
                }
                //cout << knight << ": " << posRow << ", " << posCol << endl;
            }
        }
        //result[resultIdx].first = posRow + 1;
        //result[resultIdx].second = posCol + 1;
        ++resultIdx;

        //cout << "# " << knight << ": " << posRow << ", " << posCol << endl;
    }

    return;
}

int main()
{
    ifstream file("C:/Users/aei7fe/source/HackerRank/src/KingRichardsKnights/KingRichardsKnights/Tests.txt");
    {
        int dimension = 0;
        file >> dimension;

        int commandsCount = 0;
        file >> commandsCount;
        Commands commands;

        while (commandsCount > 0)
        {
            int top, left, len;
            file >> top >> left >> len;
            commands.push_back({top, left, len});

            --commandsCount;
        }

        int knightsCount = 0;
        file >> knightsCount;
        Knights knights;

        while (knightsCount > 0)
        {
            int knight;
            file >> knight;
            knights.push_back(knight);
            --knightsCount;
        }

        tests.push_back({dimension, commands, knights});
    }
    
    for (auto test : tests)
    {
        auto&& knights = std::get<2>(test);
        
        //vector<pair<int,int>> result(knights.size(), { 0,0 });
        vector<pair<int, int>> result(1, { 0,0 });

        kingRichardKnights(std::get<0>(test), std::get<1>(test), knights, result);
        /*
        cout << results.size() << endl;
        
        for (auto result : results)
        {
            for (auto item : result)
            {
                cout << item << ' ';
            }
            cout << endl;
        }
        cout << "---------------" << endl;
        */
    }
    return 0;
}
