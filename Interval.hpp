#include <iostream>
#include <fstream>
using namespace std;

class Interval
{
private:
    int l, u;
    unsigned int key;
    char c;
public:
    Interval(int lBound = 0, int uBound = 0, unsigned int k = 0, char sym = 0)
    {
        l = lBound;
        u = uBound;
        key = k;
        c = sym;
    }
    ~Interval() = default;
    int getL()
    {
        return l;
    }
    int getU()
    {
        return u;
    }
    unsigned int getKey() const
    {
        return key;
    }
    char getC()
    {
        return c;
    }
    void setL(int newL)
    {
        l = newL;
    }
    void setU(int newU)
    {
        u = newU;
    }
    void setKey(unsigned int k)
    {
        key = k;
    }
    void setC(char newC)
    {
        c = newC;
    }
    friend ostream & operator<< (ostream &out, Interval &that)
    {
        out << "Interval for symbol <" << that.c << "> is [" << that.l << "; " << that.u << ") and key = " << that.key << endl;
        return out;
    }
};

struct check
{
    bool operator() (const Interval i1, const Interval i2) const
    {
        return i1.getKey() > i2.getKey();
    }
};
