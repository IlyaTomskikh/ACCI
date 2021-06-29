#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <cstdio>
#include "Interval.hpp"
using namespace std;

typedef unsigned short ushort;

string comp = "compressedText.txt",
decomp = "decompressedText.txt",
original = "originalText.txt";

const ushort qtr1 = (65535 + 1) / 4, half = qtr1 * 2, qtr3 = qtr1 * 3;

map<char, int> tabCreater(string txtFile)
{
    ifstream fin(txtFile, ios::binary);
    map<char, int> tab;
    if (fin.is_open())
    {
        while(1)
        {
            unsigned char c = fin.get();
            if (fin.eof()) break;
            tab[c]++;
        }
    }
    fin.close();
    return tab;
}

void zip(list<Interval> intervals, map<char, int> tab)
{
    ofstream compressed(comp, ios::binary);
    int tabSize = tab.size();
    //frequencies -> compressed file
    compressed.write((char*)&tabSize, sizeof(tabSize));
    for (map<char, int>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        compressed.write((char*)&iter->first, sizeof(iter->first));
        compressed.write((char*)&iter->second, sizeof(iter->second));
    }
    ushort l = 0, u = 65535;
    int divisor = intervals.back().getU(),
    bitsToFollow = 0,
    counter = 0;
    ifstream orig(original, ios::binary);
    if (!orig.is_open()) exit(666);
    char toWrite = 0;
    while(!orig.eof())//swap 1
    {
        int current = orig.get();
        if (current == EOF) break;
        list<Interval>::iterator iter = intervals.begin();
        for (; iter != intervals.end() && iter->getC() != current; ++iter);
        int range = u - l + 1;
        u = l + iter->getU() * range / divisor - 1;
        l += iter->getL() * range / divisor;
        while(1)
        {
            if (u < half)
            {
                ++counter;
                if (counter == 8)
                {
                    compressed.put(toWrite);
                    toWrite ^= toWrite;
                    counter ^= counter;
                }
                for(; bitsToFollow > 0; --bitsToFollow)
                {
                    toWrite |= (1 << (7 - counter));
                    ++counter;
                    if (counter == 8)
                    {
                        compressed.put(toWrite);
                        toWrite ^= toWrite;
                        counter ^= counter;
                    }
                }
            }
            else if(l >= half)
            {
                toWrite |= (1 << (7 - counter));
                ++counter;
                if (counter == 8)
                {
                    compressed.put(toWrite);
                    toWrite ^= toWrite;
                    counter ^= counter;
                }
                for(; bitsToFollow > 0; --bitsToFollow)
                {
                    ++counter;
                    if (counter == 8)
                    {
                        compressed.put(toWrite);
                        toWrite ^= toWrite;
                        counter ^= counter;
                    }
                }
                u -= half;
                l -= half;
            }
            else if (l >= qtr1 && u < qtr3)
            {
                ++bitsToFollow;
                u -= qtr1;
                l -= qtr1;
            }
            else break;
            u <<= 1;
            l <<= 1;
            ++u;
        }
    }
    orig.close();
    compressed.close();
}

void unzip(list<Interval> intervalsToUnzip, int fullLen, ifstream &compressedF)
{
    
    ofstream decompressed(decomp, ios::binary);
    int divisor = intervalsToUnzip.back().getU(),
    val = (compressedF.get() << 8) | compressedF.get(),
    counter = 0;
    char toDec = compressedF.get();
    ushort l = 0, u = 65535;
    while(fullLen)
    {
        list<Interval>::iterator iter = intervalsToUnzip.begin();
        unsigned int freq = (((val - l + 1) * divisor) - 1) / (u - l + 1);
        for (; iter->getU() <= freq; ++iter);
        int range = u - l + 1;
        u = l + iter->getU() * range / divisor - 1;
        l += iter->getL() * range / divisor;
        while (1)
        {
            if (u < half)
            {}
            else if (l >= half)
            {
                val -= half;
                l -= half;
                u -= half;
            }
            else if (l >= qtr1 && u < qtr3)
            {
                val -= qtr1;
                l -= qtr1;
                u -= qtr1;
            }
            else break;
            l <<= 1;
            u <<= 1;
            ++u;
            val <<= 1;
            val += (toDec & (1 << (7 - counter))) >> (7 - counter);
            ++counter;
            if (counter == 8)
            {
                counter ^= counter;
                toDec = compressedF.get();
            }
        }
        decompressed << iter->getC();
        --fullLen;
    }
    compressedF.close();
    decompressed.close();
}

int main()
{
    map<char, int> tab = tabCreater(original);
    list<Interval> intervals;
    for (map<char, int>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        Interval tmpI(0, 0, iter->second, iter->first);
        intervals.push_back(tmpI);
    }
    intervals.sort(check());
    int tmpBound = 0;
    for (list<Interval>::iterator iter = intervals.begin(); iter != intervals.end(); ++iter)
    {
        iter->setL(tmpBound);
        tmpBound += iter->getKey();
        iter->setU(tmpBound);
    }
    zip(intervals, tab);
    std::cout << "file '" << original << "' has been compressed and written in '" << comp << "' file" << endl;
    //compression ends here

    int fullLen = 0;
    ifstream compressedF(comp, ios::binary);
    map<char, int> freqs;
    int tabSize, key;
    char c;
    compressedF.read((char*)&tabSize, sizeof(tabSize));
    while(tabSize)
    {
        compressedF.read((char*)&c, sizeof(c));
        compressedF.read((char*)&key, sizeof(key));
        fullLen += key;
        freqs[c] = key;
        --tabSize;
    }
    list<Interval> intervalsToUnzip;
    for (map<char, int>::iterator iter = freqs.begin(); iter != freqs.end(); ++iter)
    {
        Interval tmpI(0, 0, iter->second, iter->first);
        cout << tmpI;
        intervalsToUnzip.push_back(tmpI);
    }
    for (auto iter = intervalsToUnzip.begin(); iter != intervalsToUnzip.end(); ++iter)
        std::cout << *iter;
    intervalsToUnzip.sort(check());
    tmpBound ^= tmpBound;
    for (auto iter = intervalsToUnzip.begin(); iter != intervalsToUnzip.end(); ++iter)
    {
        iter->setL(tmpBound);
        tmpBound += iter->getKey();
        iter->setU(tmpBound);
    }
    unzip(intervalsToUnzip, fullLen, compressedF);
    std::cout << "file '" << comp << "' has been decompressed and written in '" << decomp << "' file" << endl;
    return 777;
}
