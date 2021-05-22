using namespace std;

class Interval
{
private:
    double l, h;
    unsigned char c;
public:
    Interval(double lBound, double uBound, unsigned char sym):l(lBound), h(uBound), c(sym){}
    ~Interval() = default;
    double getL()
    {
        return l;
    }
    double getH()
    {
        return h;
    }
    unsigned char getC()
    {
        return c;
    }
    void setL(double newL)
    {
        l = newL;
    }
    void seth(double newH)
    {
        h = newH;
    }
    void setC(unsigned char newC)
    {
        c = newC;
    }
};
