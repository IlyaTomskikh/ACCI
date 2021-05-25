#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include "Interval.hpp"
using namespace std;

typedef unsigned char uchar;
typedef long double ldouble;

map<uchar, ldouble> freqs;
string comp = "compressedText.txt",
decomp = "decompressedText.txt",
original = "originalText.txt",
frequency = "frequency.txt";

map<uchar, ldouble> alphabetReader(int *size)
{
    ifstream orig(original);
    int counter = 0;
    map<uchar, ldouble> tab;
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

list<Interval> freqsCreater(map<uchar, ldouble> tab, int fileSize)
{
    list<Interval> intervals;
    ldouble tmp = 0;
    ofstream frequencyFile(frequency);
    for (map<uchar, ldouble>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        freqs[iter->first] = (iter->second) / fileSize;
        frequencyFile << iter->first << freqs[iter->first];
        intervals.push_back(Interval(tmp, tmp + iter->second / fileSize, iter->first));
        tmp += iter->second / fileSize;
    }
    frequencyFile.close();
    return intervals;
}

void mapSort(map<uchar, ldouble> *tab)
{
    list<ldouble> tmpToSort;
    for (map<uchar, ldouble>::iterator iter = tab->begin(); iter != tab->end(); ++iter)
        tmpToSort.push_back(iter->second);
    tmpToSort.sort();
    for (map<uchar, ldouble>::iterator iter = tab->begin(); iter != tab->end(); ++iter)
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

void zip(map<uchar, Interval> intervals)
{
    ifstream input(original);
    ofstream output(comp);
    uchar _ch;
    ldouble l = 0, u = 1, res = 0, tmpRes = 0;
    string result = "";
    while(1)
    {
        _ch = input.get();
        if (input.eof()) break;
        //result.push_back(_ch);
        /*
        cout << "interval for '" << _ch <<
        "' is [" << intervals[_ch].getL() <<
        "; " << intervals[_ch].getU() << ')' <<
        endl << "in other words: lower bound = " << intervals[_ch].getL() <<
        "; upper bound = " << intervals[_ch].getU() << endl <<
        "current l = " << l << "; current u = " << u << endl;
        */
        ldouble new_u = intervals[_ch].getU() * (u - l) + l;
        ldouble new_l = intervals[_ch].getL() * (u - l) + l;
        u = new_u;
        l = new_l;
        res = (u + l) / 2;
        //cout << "u = " << u << "; l = " << l << "; tmpRes = " << tmpRes << endl;
        //system("pause");
        //cout << '{' << result << "} res = " << res << endl;
        cout << "symbol '" << _ch << "' and current result is" << res << endl;
    }
    input.close();
    output << res;
    output.close();
}

int main()
{
    int fileSize = 0;
    map<uchar, ldouble> tab = alphabetReader(&fileSize);
    //cout << "fileSize = " << fileSize << endl;
    list<Interval> list_of_intervals = freqsCreater(tab, fileSize);
    map<uchar, Interval> intervals = mapCreater(list_of_intervals);
    for (map<uchar, Interval>::iterator iter = intervals.begin(); iter != intervals.end(); ++iter)
    {
        cout << iter->second;
    }
    zip(intervals);
    return 777;
}
