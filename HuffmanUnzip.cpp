#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
using namespace std;

vector<bool> hufCode;
map<char, vector<bool> > trueTab;
string compressed = "compressedText.txt",
uncompressed = "uncompressedText.txt";
ifstream treeFile("tree.txt", ios::in | ios::binary);
long int sizeOfFile = 0;

class Node
{
    public:
    int key;
    char ch;
    Node *left;
    Node *right;
    
    Node()
    {
        key = 0;
        ch = 0;
        left = right = nullptr;
    }

    Node(Node *l, Node *r)
	{
        left =  l;
        right = r;
        key = l->key + r->key;
    }

    int unzip()
    {
        ifstream enc(compressed, ios::in | ios::binary);
        if (!enc.is_open()) return -1;
        ofstream dec(uncompressed, ios::out | ios::binary);
        Node *tmp = this;
        int counter = 0;
        char buff = enc.get();
        while (!enc.eof())
        {
            bool flag = (buff & (1 << (7 - counter)));
            if (flag) tmp = tmp->right;
            else tmp = tmp->left;
            if (tmp->left==nullptr && tmp->right==nullptr)
            {
                dec << tmp->ch;
                tmp = this;
            }
            ++counter;
            if (counter == 8)
            {
                counter ^= counter;
                buff = enc.get();
            }
        }
        enc.close();
        dec.close();
        return 1;
    }
};

struct check
{
    bool operator() (const Node *l, const Node *r) const
    {
        return l->key < r->key;
    }
};

int fileSize(string add)
{
    ifstream mySource(add, ios_base::binary);
    mySource.seekg(0,ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}

void deleteLastByte(string fileName)
{
    ifstream file(fileName);
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();
    content.pop_back();
    ofstream _file(fileName, std::ios::trunc);
    _file << content;
    _file.close();
}

map<char, int> treeCreater()
{
    map<char, int> tab;
    int number, _key;
    treeFile >> number;
    char _ch;
    cout << "pairs number = " << number << endl;
    while(number != 0)
    {
        treeFile >> _ch;
        treeFile >> _key;
        tab[_ch] = _key;
        --number;
    }
    return tab;
}

int main()
{
    map<char, int> tab = treeCreater();
    list<Node*> tree;
    for (map<char, int>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        cout << "tab[" << iter->first << "] = " << iter->second << endl;
        Node *tmp = new Node;
        tmp->ch = iter->first;
        tmp->key = iter->second;
        tree.push_back(tmp);
    }
    while (tree.size() != 1)
    {
        tree.sort(check());
        Node *l = tree.front();
        tree.pop_front();
        Node *r = tree.front();
        tree.pop_front();
        Node *batya = new Node(l, r);
        tree.push_back(batya);
    }
    Node *root = tree.front();
    char ch__ = 0;
    for (int i = 10; i--;) treeFile >> ch__;
    treeFile >> sizeOfFile;
    treeFile.close();
    int test = root->unzip();
    if (test == -1)
    {
        cout << "Couldn't open file " << compressed << endl;
        return 666;
    }
    cout << "size of file must be " << sizeOfFile << " bytes" << endl;
    long fS = fileSize(uncompressed);
    while(fS > sizeOfFile)
    {
        cout << "current file size = " << fS << endl;
        deleteLastByte(uncompressed);
        fS = fileSize(uncompressed);
    }
    cout << "File "<< compressed << " was uncompressed and rewritten to file uncomressedText.txt" << endl;
    return 777;
}
