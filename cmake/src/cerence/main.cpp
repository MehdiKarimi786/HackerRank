// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>

struct Node {
    using Type = int;

    Type val_;
    Node* next_;

    Node(const Type& val) : val_(val), next_(nullptr) {}
};

struct MyNode {
    using Type = Node::Type;

    std::size_t sz_;
    Node* head_;
    Node* tail_;

    MyNode() : sz_(0), head_(nullptr), tail_(nullptr){}
    MyNode(Type val) : sz_(1), head_(new Node(val)), tail_(head_){}
    MyNode(const std::initializer_list<Type>& vals)
        : sz_(1), head_(new Node(*vals.begin())), tail_(head_)
    {
        for (auto it = vals.begin() + 1; it != vals.end(); ++it)
        {
            push_back(*it);
        }
    }

    inline bool empty() const noexcept
    {
        return head_ == nullptr;
    }

    Type& operator [](int idx) noexcept
    {
        int curIdx = 0;
        auto curNode = head_;
        while (curIdx < idx)
        {
            curNode = curNode->next_;
            ++curIdx;
        }
        return curNode->val_;
    }

    Type operator [](int idx) const noexcept
    {
        int curIdx = 0;
        auto curNode = head_;
        while (curIdx < idx)
        {
            curNode = curNode->next_;
            ++curIdx;
        }
        return curNode->val_;
    }

    std::size_t size() const noexcept
    {
        return sz_;
    }

    void print() const noexcept
    {
        auto cur = head_;
        while (cur)
        {
            std::cout << cur->val_ << " , ";
            cur = cur->next_;
        }
        return;
    }

    void push_back(const Type& val) noexcept
    {
        if (head_ == nullptr)
        {
            head_ = new Node(val);
            tail_ = head_;
            sz_ = 1;
        }
        else if (tail_)
        {
            tail_->next_ = new Node(val);
            tail_ = tail_->next_;
            ++sz_;
        }
    }

    void clear()
    {
        head_ = nullptr;
        tail_ = nullptr;
        sz_ = 0;
    }

    void append(const MyNode& node, std::size_t endIndex)
    {
        if (node.size() < endIndex)
            return;
        int idx = 0;
        auto cur = node.head_;
        while (idx < endIndex)
        {
            push_back(cur->val_);

            ++idx; cur = cur->next_;
        }
        

    }
};


using VectorRoute = std::vector<int>;

//#define VECTOR_ROUTE
#ifdef VECTOR_ROUTE
using Route = VectorRoute;
#else
using Route = MyNode;
#endif

using Routes = std::vector<Route>;
using Index = int;
const Index InvalidIndex = -1;

Index getConnectPoint(const Route& curRoute, Index curIdx, const Routes& AllRoutes)
{
    Index resultIdx = InvalidIndex;

    if (curRoute.empty() /* cur route is empty*/)
    {
        resultIdx = InvalidIndex;
    }
    else if (curIdx == AllRoutes.size() - 1 /*last route in the list*/)
    {
        resultIdx = InvalidIndex;
    }
    else if (AllRoutes[curIdx + 1].empty() /* next route is empty*/)
    {
        resultIdx = InvalidIndex;
    }
    else
    {
        auto headNextRoute = AllRoutes[curIdx + 1][0];
        auto sz = curRoute.size();
        for (Index i = sz - 1; i >= 0 && resultIdx == InvalidIndex; --i)
        {
            if (curRoute[i] < headNextRoute)
            {
                resultIdx = i;
            }
        }
    }
    return resultIdx;
}

template<typename Routes>
void printRoutes(const Routes& routes)
{
    for (auto route : routes)
    {
        std::cout << ". ";
        for (auto val : route)
            std::cout << val << " , ";
        std::cout << std::endl;
    }
}

template<>
void printRoutes(const std::vector<MyNode>& routes)
{
    for (auto route : routes)
    {
        std::cout << ". ";
        route.print();
        std::cout << std::endl;
    }
}

int main()
{
    Routes result;
    Route cur;

    auto routes = Routes{ {10,1,2,4,5,6,3,3}, {},{3,5},{2,1},{4,1,2,3}, {1} };
    auto sz = routes.size();
    for (Index idx(0); idx < sz; ++idx)
    {
        auto&& curRoute = routes[idx];
        auto&& curSz = curRoute.size();
        auto connectPoint = getConnectPoint(curRoute, idx, routes);
        if (connectPoint != InvalidIndex)
        {
#ifdef VECTOR_ROUTE
            cur.insert(cur.end(), curRoute.begin(), curRoute.begin() + connectPoint + 1);
#else
            cur.append(curRoute, connectPoint + 1);
#endif
        }
        else
        {
#ifdef VECTOR_ROUTE
            cur.insert(cur.end(), curRoute.begin(), curRoute.end());
#else
            cur.append(curRoute, curSz);
#endif
            if (!cur.empty())
            {
                result.push_back(cur);
            }
            cur.clear();
        }
    }

    if (!cur.empty())
    {
        result.push_back(cur);
    }

    std::cout << "--------" << std::endl;
    printRoutes(routes);
    std::cout << "--------" << std::endl;
    printRoutes(result);
    std::cout << "--------" << std::endl;
    return 0;
}
