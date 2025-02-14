// MagicSpell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>

#define DEBIUG_PRINT


using namespace std;

ifstream file("C:/Users/aei7fe/source/HackerRank/HackerRank/MagicSpell/Tests.txt");

class Spell {
private:
    string scrollName;
public:
    Spell() : scrollName("") { }
    Spell(string name) : scrollName(name) { }
    virtual ~Spell() { }
    string revealScrollName() {
        return scrollName;
    }
};

class Fireball : public Spell {
private: int power;
public:
    Fireball(int power) : power(power) { }
    void revealFirepower() {
        cout << "Fireball: " << power << endl;
    }
};

class Frostbite : public Spell {
private: int power;
public:
    Frostbite(int power) : power(power) { }
    void revealFrostpower() {
        cout << "Frostbite: " << power << endl;
    }
};

class Thunderstorm : public Spell {
private: int power;
public:
    Thunderstorm(int power) : power(power) { }
    void revealThunderpower() {
        cout << "Thunderstorm: " << power << endl;
    }
};

class Waterbolt : public Spell {
private: int power;
public:
    Waterbolt(int power) : power(power) { }
    void revealWaterpower() {
        cout << "Waterbolt: " << power << endl;
    }
};

class SpellJournal {
public:
    static string journal;
    static string read() {
        return journal;
    }
};
string SpellJournal::journal = "";

void counterspell(Spell* spell) {

    /* Enter your code here */
    {
        auto sp = dynamic_cast<Fireball*>(spell);
        if (sp)
        {
            sp->revealFirepower();
            return;
        }
    }

    {
        auto sp = dynamic_cast<Frostbite*>(spell);
        if (sp)
        {
            sp->revealFrostpower();
            return;
        }
    }

    {
        auto sp = dynamic_cast<Waterbolt*>(spell);
        if (sp)
        {
            sp->revealWaterpower();
            return;
        }
    }

    {
        auto sp = dynamic_cast<Thunderstorm*>(spell);
        if (sp)
        {
            sp->revealThunderpower();
            return;
        }
    }

    auto reveal = spell->revealScrollName();
    int lenReveal = reveal.length();
    auto journal = SpellJournal::read();
    int lenJournal = journal.length();

    vector<vector<int>> allPositions;

    for (auto i(0); i < lenReveal; ++i)
    {
        vector<int> positions;

        auto ch = reveal[i];
        auto foundPos = -1;
        do {
            foundPos = journal.find(ch, foundPos + 1);
            if ((size_t)foundPos == string::npos)
            {
                break;
            }
            positions.push_back(foundPos);
        } while (1);

        if (!positions.empty())
        {
            allPositions.push_back(positions);
        }
    }

    int sz = allPositions.size();
    struct Node {
        Node(int row, int col, int pos):
            row_(row),
            col_(col),
            pos_(pos)
        {}
        int row_;
        int col_;
        int pos_;
        vector<Node> nodes_;
    };

    function<Node(int, int, int)> getNode;
    getNode = [&](int curRow, int curCol, int curPos) {
        auto curNode = Node(curRow, curCol, curPos);
        for (auto nextCol(curCol + 1); nextCol < sz; ++nextCol)
        {
            auto && nextPositions = allPositions[nextCol];
            int nextSz = nextPositions.size();
            for (auto nextRow(0); nextRow < nextSz; ++nextRow)
            {
                auto nextPos = allPositions[nextCol][nextRow];
                if (curPos < nextPos)
                {
                    auto nextNode = getNode(nextRow, nextCol, nextPos);
                    curNode.nodes_.push_back(nextNode);
                    break;
                }
            }
        }
        return curNode;
    };

    function<int(Node)> getMaxDepth;
    getMaxDepth = [&](Node node) {
        auto result = 0;
        for (auto ch : node.nodes_)
        {
            result = max<int>(result, getMaxDepth(ch));
        }
        return result + 1;
    };

    function<int(int, int, int)> getMaxDepth2;
    getMaxDepth2 = [&](int curRow, int curCol, int curPos) {
        auto result = 0;
        for (auto nextCol(curCol + 1); nextCol < sz; ++nextCol)
        {
            auto&& nextPositions = allPositions[nextCol];
            int nextSz = nextPositions.size();
            for (auto nextRow(0); nextRow < nextSz; ++nextRow)
            {
                auto nextPos = allPositions[nextCol][nextRow];
                if (curPos < nextPos)
                {
                    result = max<int>(result, getMaxDepth2(nextRow, nextCol, nextPos));
                    break;
                }
            }
        }
        return result + 1;
    };

    auto maxDepth = 0;
    for (auto curCol(0); curCol < sz;++curCol)
    {
        auto curRow = 0;
        auto curPos = allPositions[curCol][curRow];
        /*
        auto curNode = getNode(curRow, curCol, curPos);
        maxDepth = max<int>(maxDepth, getMaxDepth(curNode));
        */
        maxDepth = max<int>(maxDepth, getMaxDepth2(curRow, curCol, curPos));
    }

    cout << maxDepth << endl;
    return;
}

class Wizard {
public:
    Spell* cast() {
        Spell* spell;
        string s; file >> s;
        int power; file >> power;
        if (s == "fire") {
            spell = new Fireball(power);
        }
        else if (s == "frost") {
            spell = new Frostbite(power);
        }
        else if (s == "water") {
            spell = new Waterbolt(power);
        }
        else if (s == "thunder") {
            spell = new Thunderstorm(power);
        }
        else {
            spell = new Spell(s);
            file >> SpellJournal::journal;
        }
        return spell;
    }
};

int main() {
    int T;
    file >> T;
    Wizard Arawn;
    while (T--) {
        Spell* spell = Arawn.cast();
        counterspell(spell);
    }
    return 0;
}