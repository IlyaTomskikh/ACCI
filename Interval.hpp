#include <iostream>
using namespace std;

class Interval
{
private:
    long double l, u;
    unsigned char c;
public:
    Interval(long double lBound, long double uBound, unsigned char sym)
    {
        l = lBound;
        u = uBound;
        c = sym;
    }
    Interval()
    {
        l = u = 0;
        c = 0;
    }
    ~Interval() = default;
    long double getL()
    {
        return l;
    }
    long double getU()
    {
        return u;
    }
    unsigned char getC()
    {
        return c;
    }
    void setL(long double newL)
    {
        l = newL;
    }
    void setU(long double newU)
    {
        u = newU;
    }
    void setC(unsigned char newC)
    {
        c = newC;
    }
    friend ostream & operator<< (ostream &out, Interval that)
    {
        out << "Interval for symbol <" << that.c << "> is [" << that.l << "; " << that.u << ')' << endl;
        return out;
    }
};
