#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include "Interval.hpp"
using namespace std;

typedef unsigned char uchar;

map<uchar, double> freqs;
string comp = "compressedText.txt", decomp = "decompressedText.txt", original = "originalText.txt";

map<uchar, double> alphabetReader(int *size)
{
    ifstream orig(original);
    int counter = 0;
    map<uchar, double> tab;
    if(orig.is_open())
    {
        while(true)
        {
            uchar _ch = orig.get();
            if (orig.eof()) break;
            tab[_ch]++;
            //cout << _ch;
            ++counter;
        }
        cout << endl;
    }
    orig.close();
    *size = counter;
    return tab;
}

list<Interval> freqsCreater(map<uchar, double> tab, int fileSize)
{
    list<Interval> intervals;
    double tmp = 0;
    for (map<uchar, double>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        freqs[iter->first] = (iter->second) / fileSize;
        cout << "freqs[" << iter->first << "] = " << freqs[iter->first] << endl;
        intervals.push_back(Interval(tmp, tmp + iter->second / fileSize, iter->first));
        tmp += iter->second / fileSize;
    }
    return intervals;
}

void mapSort(map<uchar, double> *tab)
{
    list<double> tmpToSort;
    for (map<uchar, double>::iterator iter = tab->begin(); iter != tab->end(); ++iter)
        tmpToSort.push_back(iter->second);
    tmpToSort.sort();
    for (map<uchar, double>::iterator iter = tab->begin(); iter != tab->end(); ++iter)
    {
        (*tab)[iter->first] = tmpToSort.back();
        tmpToSort.pop_back();
    }
}

int main()
{
    int fileSize = 0;
    map<uchar, double> tab = alphabetReader(&fileSize);
    cout << "fileSize = " << fileSize << endl;
    list<Interval> intervals = freqsCreater(tab, fileSize);
    for(list<Interval>::iterator iter = intervals.begin(); iter != intervals.end(); ++iter)
        cout << "interval for symbol "<< iter->getC() << " [" << iter->getL() << "; " << iter->getH() << ')' << endl;
    
    return 777;
}
