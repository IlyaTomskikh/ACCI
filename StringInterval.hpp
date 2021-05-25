#include <string>
#include "Interval.hpp"
using namespace std;

class StringInterval
{
private:
    long double lBound, uBound;
    string symbols;
public:
    StringInterval(long double l, long double u, string newSymbols = "");
    StringInterval(Interval that);
    StringInterval()
    {
        lBound = uBound = 0;
        symbols = "";
    }
    ~StringInterval() = default;
    
    StringInterval operator+=(Interval that)
    {
        if(uBound == 0)
        {
            lBound = that.getL();
            uBound = that.getU();
            symbols.push_back(that.getC());
        }
        else
        {
            symbols.push_back(that.getC());
            long double range = lBound - uBound;
            lBound = lBound + that.getL() * range;
            uBound = lBound + that.getU() * range;
        }
        return *this;
    }
    StringInterval operator=(Interval that)
    {
        lBound = that.getL();
        uBound = that.getU();
        symbols = "";
        symbols.push_back(that.getC());
        return *this;
    }

    long double getL()
    {
        return lBound;
    }
    long double getU()
    {
        return uBound;
    }
};

StringInterval::StringInterval(long double l, long double u, string newSymbols)
{
    lBound = l;
    uBound = u;
    int i = 0;
    do
    {
        symbols.push_back(newSymbols[i]);
        ++i;
    }while(i < newSymbols.size());
}

StringInterval::StringInterval(Interval that): lBound(that.getL()), uBound(that.getU())
{
    symbols.push_back(that.getC());
}
