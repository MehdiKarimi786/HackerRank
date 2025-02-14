#include <iostream>
#include <vector>

using namespace std;

template <bool...digits>
int reversed_binary_value() {
    bool arr[] = { digits... };
    int sum = 0;
    int a = 0;
    for (bool i : arr) {
        sum = sum | ((1 & i) << a++);
    }
    return sum;
}

/*
template<bool F, bool ...I>
int reversed_binary_value()
{
    return F + 2 * reversed_binary_value<I...>();
}

template<>
int reversed_binary_value<0>()
{
    return 0;
}

template<>
int reversed_binary_value<1>()
{
    return 1;
}
*/
//template<bool...digits> int reversed_binary_value();
/*
template<bool b1, bool b2, bool b3, bool b4, bool b5, bool b6> int reversed_binary_value()
{
    static vector<int> pow2{1, 2, 4, 8, 16, 32 };
    return (b1?pow2[0]: 0) + 
        (b2 ? pow2[1] : 0) + 
        (b3 ? pow2[2] : 0) + 
        (b4 ? pow2[3] : 0) + 
        (b5 ? pow2[4] : 0) + 
        (b6 ? pow2[5] : 0);
}
*/

template <int n, bool...digits>
struct CheckValues {
    static void check(int x, int y)
    {
        CheckValues<n - 1, 0, digits...>::check(x, y);
        CheckValues<n - 1, 1, digits...>::check(x, y);
    }
};

template <bool...digits>
struct CheckValues<0, digits...> {
    static void check(int x, int y)
    {
        int z = reversed_binary_value<digits...>();
        std::cout << (z + 64 * y == x);
    }
};



enum class Fruit { apple, orange, pear };
enum class Color { red, green, orange };
template <typename T> struct Traits;

template <> struct Traits<Fruit> {
    static string name(int index)
    {
        switch (index)
        {
        case (int)Fruit::apple:
            return "apple";
        case (int)Fruit::orange:
            return "orange";
        case (int)Fruit::pear:
            return "pear";

        default:
            return "unknown";
        }
    }
};

template <> struct Traits<Color> {
    static string name(int index)
    {
        switch (index)
        {
        case (int)Color::green:
            return "green";
        case (int)Color::orange:
            return "orange";
        case (int)Color::red:
            return "red";

        default:
            return "unknown";
        }
    }
};



int main()
{
    cout << Traits<Color>::name(1) << " ";
    cout << Traits<Fruit>::name(0) << "\n";

    cout << Traits<Color>::name(3) << " ";
    cout << Traits<Fruit>::name(3) << "\n";
    
    CheckValues<6>::check(65, 1);
}

