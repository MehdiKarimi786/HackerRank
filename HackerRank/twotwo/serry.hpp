#pragma once

#include <vector>
#include <array>

using Contianer = std::vector<int>;
Contianer getValues(int val);

class SerryB {
public:
    SerryB(int val)
    {
        auto values = getValues(val);
        std::fill(counts_.begin(), counts_.end(), 0);
        for (auto v : values)
        {
            counts_[v] = 1;
        }
    }

    void multiple(int index, int cnt) 
    {
        auto& vvv = counts_[index];
        auto curCnt = vvv + cnt;
        if (curCnt > 160000000)
        {
            //if (curCnt % 2 == 0)
            if ((curCnt & 1) == 0)
            {
                vvv = 0;
            }
            else
            {
                vvv = 1;
            }
            auto ddd = curCnt >> 1;
            counts_[index + 1] += ddd;
        }
        else
        {
            vvv = curCnt;
        }

        return;
    }

    bool action(const Contianer& s)
    {
        auto sz = counts_.size();
        for (int i(sz - 4); i >= 0; --i)
        {
            auto cnt = counts_[i];
            
            if (!cnt)
                continue;
            
            if(i > 800)
                return false;

            multiple(i + 1, cnt);
            multiple(i + 3, cnt);

            counts_[i] = 0;
        }
        for (auto l : s)
        {
            multiple(l, 1);
        }
        return true;
    }

    bool isPowOf2()
    {
        auto sz = counts_.size();
        auto sum = 0;
        for (int i(0); i < sz; ++i)
        {
            auto cnt = counts_[i];
            if (cnt == 0)
            {
                continue;
            }
            //else if (cnt %2 == 0)
            else if ((cnt & 1) == 0)
            {
                counts_[i] = 0;
                counts_[i + 1] = counts_[i + 1] + cnt / 2;
            }
            else
            {
                ++sum;
                if (sum > 1)
                {
                    return false;
                }
                counts_[i] = 1;
                counts_[i + 1] = counts_[i + 1] + (cnt - 1) / 2;
            }
        }

        return sum == 1 || sum == 0;
    }
#ifdef DEBUG
    long long value() const noexcept
    {
        long long v = 0;
        long long pw = 1;
        for (auto i(0u); i < counts_.size(); ++i)
        {
            v += counts_[i] * pw;
            pw = pw * 2;
        }
        return v;
    }
#endif
private:
    std::array<int, 1000> counts_;
    int maxIndex_ = 0;
};
