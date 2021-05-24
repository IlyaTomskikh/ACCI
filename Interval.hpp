#include <iostream>
using namespace std;

class Interval
{
private:
    double l, u;
    unsigned char c;
public:
    Interval(double lBound, double uBound, unsigned char sym)
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
    double getL()
    {
        return l;
    }
    double getU()
    {
        return u;
    }
    unsigned char getC()
    {
        return c;
    }
    void setL(double newL)
    {
        l = newL;
    }
    void setU(double newU)
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
