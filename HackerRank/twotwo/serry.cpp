#pragma once

#include "serry.hpp"

Contianer getValues(int val)
{
    Contianer values;
    if (val == 1)
    {
        values = { 0 };
    }
    else if (val == 2)
    {
        values = { 1 };
    }

    else if (val == 3)
    {
        values = { 1, 0 };
    }

    else if (val == 4)
    {
        values = { 2 };
    }

    else if (val == 5)
    {
        values = { 2, 0 };
    }

    else if (val == 6)
    {
        values = { 2,1 };
    }

    else if (val == 7)
    {
        values = { 2, 1, 0 };
    }

    else if (val == 8)
    {
        values = { 3 };
    }

    else if (val == 9)
    {
        values = { 3, 0 };
    }

    else if (val == 10)
    {
        values = { 3, 1 };
    }

    return values;
}



class SerryA {
public:
    SerryA(int val)
    {
        auto values = getValues(val);
        std::fill(counts_.begin(), counts_.end(), 0);
        for (auto v : values)
        {
            counts_[v] = 1;
        }
        size_ = values.size();
    }

    void action(const Contianer& s)
    {
        auto multiple = [&](int index) {
            int size = size_;
            while (counts_[index])
            {
                counts_[index] = 0;
                --size;
                ++index;
            }
            size_ = size + 1;
            counts_[index] = 1;
        };
        auto sz = counts_.size();
        for (int i(sz - 4); i >= 0; --i)
        {
            if (!counts_[i])
            {
                continue;
            }
            counts_[i] = 0;
            --size_;
            multiple(i + 1);
            multiple(i + 3);
        }
        for (auto l : s)
        {
            multiple(l);
        }
    }

    bool isPowOf2() const noexcept
    {
        return size_ == 1;
    }

#ifdef DEBUG
    long long value() const noexcept
    {
        long long v = 0;
        long long pw = 1;
        for (auto i(0u); i < counts_.size(); ++i)
        {
            if (counts_[i])
            {
                v += pw;
            }
            pw = pw * 2;
        }
        return v;
    }
#endif

private:
    std::array<int, 12048> counts_;
    int size_ = 0;
};

class SerryBoolean {
public:
    SerryBoolean(int val)
    {
        auto values = getValues(val);
        std::fill(counts_.begin(), counts_.end(), false);
        for (auto v : values)
        {
            counts_[v] = true;
        }
        size_ = values.size();
    }

    void action(const Contianer& s)
    {
        auto multiple = [&](int index) {
            int size = size_;
            while (counts_[index])
            {
                counts_[index] = false;
                --size;
                ++index;
            }
            size_ = size + 1;
            counts_[index] = true;
        };
        auto sz = counts_.size();
        for (int i(sz - 4); i >= 0; --i)
        {
            if (!counts_[i])
            {
                continue;
            }
            counts_[i] = false;
            --size_;
            multiple(i + 1);
            multiple(i + 3);
        }
        for (auto l : s)
        {
            multiple(l);
        }
    }

    bool isPowOf2() const noexcept
    {
        return size_ == 1;
    }

#ifdef DEBUG
    long long value() const noexcept
    {
        long long v = 0;
        long long pw = 1;
        for (auto i(0u); i < counts_.size(); ++i)
        {
            if (counts_[i])
            {
                v += pw;
            }
            pw = pw * 2;
        }
        return v;
    }
#endif
private:
    std::array<bool, 12048> counts_;
    int size_ = 0;
};

