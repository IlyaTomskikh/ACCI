#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include "StringInterval.hpp"
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
            ++counter;
        }
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
        //cout << "freqs[" << iter->first << "] = " << freqs[iter->first] << endl;
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

map<uchar, Interval> mapCreater(list<Interval> list_of_intervals)
{
    map<uchar, Interval> intervals;
    for(list<Interval>::iterator iter = list_of_intervals.begin(); iter != list_of_intervals.end(); ++iter)
    {
        intervals[iter->getC()] = *iter;
    }
    return intervals;
}

double zip(map<uchar, Interval> intervals)
{
    ifstream input(original);
    uchar _ch = input.get();
    StringInterval res = intervals[_ch];
    while(!input.eof())
    {
        _ch = input.get();
        //if (input.eof()) break;
        res += intervals[_ch];
    }
    input.close();
    return res.getL();
}

int main()
{
    int fileSize = 0;
    map<uchar, double> tab = alphabetReader(&fileSize);
    //cout << "fileSize = " << fileSize << endl;
    list<Interval> list_of_intervals = freqsCreater(tab, fileSize);
    map<uchar, Interval> intervals = mapCreater(list_of_intervals);
    for (map<uchar, Interval>::iterator iter = intervals.begin(); iter != intervals.end(); ++iter)
    {
        cout << iter->second;
    }
    return 777;
}
