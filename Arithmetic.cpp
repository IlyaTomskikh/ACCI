#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <set>
using namespace std;

string alphabet = "";
int *frequency;
int *tab = new int[256];
string original = "originalText.txt";
string compressed = "compressedText.txt";
string uncompressed = "uncompressedText.txt";

void defFreqs()
{
    ifstream input(original, ios::out | ios::binary);
    while(!input.eof())
    {
        char c = input.get();
        if (alphabet.find(c)) tab[c]++;
        else
        {
            alphabet.push_back(c);
            tab[c]++;
        }
    }
    input.close();
    frequency = new int[alphabet.size()];
    for (int i = 0; i < alphabet.size(); ++i) frequency[i] = tab[alphabet[i]] / alphabet.size();
    ofstream output(compressed, ios::out | ios::binary);
}

int main()
{
    return 777;
}
