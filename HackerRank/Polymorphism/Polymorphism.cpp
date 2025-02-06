#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
#include <fstream>
using namespace std;

struct Node {
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
    Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
    map<int, Node*> mp; //map the key to the node in the linked list
    int cp;  //capacity
    Node* tail; // double linked list tail pointer
    Node* head; // double linked list head pointer
    virtual void set(int, int) = 0; //set function
    virtual int get(int) = 0; //get function

};

class LRUCache : public Cache {
public:
    LRUCache(int capacity)
    {
        cp = capacity;
        tail = head = nullptr;
    }

    void set(int key, int value)override
    {
        auto it = mp.find(key);
        if (it == mp.end())
        {
            auto node = new Node(nullptr, head, key, value);
            if (head)
            {
                head->prev = node;
            }
            head = node;

            if (tail == nullptr)
            {
                tail = node;
            }

            mp.insert({ key, node });

            if (mp.size() > cp)
            {
                node = tail;
                tail = tail->prev;
                mp.erase(node->key);
                delete node;
            }
        }
        else
        {
            auto node = it->second;
            node->value = value;
            if (node != head)
            {
                node->prev->next = node->next;

                if (node != tail)
                {
                    node->next->prev = node->prev;
                }
                else if (tail == node)
                {
                    tail = node->prev;
                    tail->next = nullptr;
                }

                node->next = head;
                head->prev = node;
                head = node;
                head->prev = nullptr;
            }
        }
        print();
    }

    void print() const noexcept
    {
        {
            auto node = head;
            while (node)
            {
                cout << node->key << " ";
                node = node->next;
            }
        }

        cout << " <--> ";

        {
            auto node = tail;
            while (node)
            {
                cout << node->key << " ";
                node = node->prev;
            }

            cout << endl;
        }
    }

    int get(int key) override
    {
        auto it = mp.find(key);
        if (it == mp.end())
        {
            return -1;
        }
        return it->second->value;
    }
};


#define SET "set"
#define GET "get"

tuple<int, vector<pair<string, int>>> test = {5,{
    {SET, 1},
    {SET, 2},
    {SET, 3},
    {SET, 4},
    {SET, 5},
    {GET, 4},
    {GET, 1},
    {GET, 2},
    {GET, 3},
    {GET, 5},
    {SET, 4},
    {SET, 1},
    {SET, 2},
    {SET, 3},
    {SET, 5},
}
} ;
int main() {
    ifstream file("C:/Users/aei7fe/source/HackerRank/HackerRank/Polymorphism/test.txt");
    
    {
        int capacity= 0;
        int commandCounts = 0;
        file >> commandCounts >> capacity;
        string command;
        int key, value;
        LRUCache l(commandCounts);
        int idx = 1;
        while (commandCounts > 0)
        {
            file >> command >> key;
            if (command == SET)
            {
                file >> value;
                cout << "set " << key << ", " << value << endl;
                l.set(key, value);
            }
            else
            {
//                cout << idx << " [get " << key << "] " << l.get(key) << endl;
            }
            ++idx;
            --commandCounts;
        }
    }

    LRUCache l(get<0>(test));
    for (auto cmd: get<1>(test)) 
    {
        auto command = cmd.first;
        auto key= cmd.second;
        auto value = key * 10;
        if (command == GET) {
            l.get(key);
        }
        else if (command == SET) {
            l.set(key, value);
        }

        cout << "[" << command << "," << key << "]: ";
        l.print();
    }
    return 0;
}
