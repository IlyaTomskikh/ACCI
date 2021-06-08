#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <cstdio>
#include "Interval.hpp"
using namespace std;

typedef unsigned char uchar;
typedef long double ldouble;

map<uchar, ldouble> freqs;
string comp = "compressedText.txt",
decomp = "decompressedText.txt",
original = "originalText.txt",
frequency = "frequency.txt";

int fs_to_dec;

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
    frequencyFile << tab.size();
    for (map<uchar, ldouble>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        freqs[iter->first] = (iter->second) / fileSize;
        frequencyFile << iter->first << iter->second;
        intervals.push_back(Interval(tmp, tmp + iter->second / fileSize, iter->first));
        tmp += iter->second / fileSize;
    }
    frequencyFile << fileSize;
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
    ofstream output(comp), cons("cons.txt");
    uchar _ch;
    ldouble l = 0, u = 1, res = 0, tmpRes = 0;
    string result = "";
    while(1)
    {
        _ch = input.get();
        if (input.eof()) break;
        result.push_back(_ch);
        
        cons << "interval for '" << _ch <<
        "' is [" << intervals[_ch].getL() <<
        "; " << intervals[_ch].getU() << ')' <<
        endl << "lower bound = " << intervals[_ch].getL() <<
        ";\nupper bound = " << intervals[_ch].getU() << endl <<
        "current l = " << l << ";\ncurrent u = " << u << endl;
        
        ldouble new_u = intervals[_ch].getU() * (u - l) + l;
        ldouble new_l = intervals[_ch].getL() * (u - l) + l;
        cons << "///recount///\nu = " << u << "; new_u = " << intervals[_ch].getU()
        << " * (" << u << " - " << l << ") + " << l << " = " << new_u << endl
        << "l = " << l << "; new_l = " << intervals[_ch].getL()
        << " * (" << u << " - " << l << ") + " << l << " = " << new_l << endl;
        if (new_u != new_l)
        {
            u = new_u;
            l = new_l;
            res = l / 2 + u / 2;    //problema v peres4ete granic (oni stanovyatsya ravny => res = 0 i zanovo)
            
        }
        else
        {
            cons << "output res" << endl;
            output << res;
            //cout << "\nres = " << res << endl;
            //printf("%Lg", res);
            res = 0.0;
        }
        printf("%Lg\n", res);
        //system("pause");
        //cout << '{' << result << "} res = " << res << endl;
        cons << "symbol '" << _ch << "' and current result is " << res << endl << endl;
    }
    input.close();
    cons.close();
    cons << "output res" << endl;
    output << res;
    output.close();
}

map<uchar, ldouble> freqsReader()
{
    map<uchar, ldouble> tab;
    ifstream fr(frequency);
    int size = fr.get();
    while(size > 0)
    {
        uchar _ch = fr.get();
        ldouble _ld = fr.get();
        tab[_ch] = _ld;
        --size;
    }
    fs_to_dec = fr.get();
    fr.close();
    for (map<uchar, ldouble>::iterator iter = tab.begin(); iter != tab.end(); ++iter) iter->second /= fs_to_dec;
    return tab;
}

void unzip(map<uchar, Interval> intervals)
{
    ofstream output(decomp);
    ldouble code = 0.0;
    for (int i = 0; i < fs_to_dec - 1; ++i)
    {
        for (int j = 0; j < intervals.size() - 1; ++j)
        {
            if (code >= intervals[j].getL() && code >= intervals[j].getU())
            {
                output << intervals[j].getC();
                code = (code - intervals[j].getL()) / (intervals[j].getU() - intervals[j].getL());
                break;
            }
        }
    }
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
    cout << "zip" << endl;
    zip(intervals);
    tab = freqsReader();
    list<Interval> listForDec = freqsCreater(tab, fs_to_dec);
    map<uchar, Interval> intervals_to_dec = mapCreater(listForDec);
    for (map<uchar, Interval>::iterator iter = intervals_to_dec.begin(); iter != intervals_to_dec.end(); ++iter)
    {
        cout << iter->second;
    }
    cout << "unzip" << endl;
    unzip(intervals_to_dec);
    return 777;
}
