#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
using namespace std;

string compressed = "compressedText.txt",
uncompressed = "uncompressedText.txt";
ifstream treeFile("tree.txt", ios::in | ios::binary);

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

    friend ostream & operator<< (ostream &out, const Node &that)
    {
        if (that.ch == 10) return out << "[new_line] = " << that.key << endl;
        else return out << '[' << that.ch << "] = " << that.key << endl;
    }

    int unzip()
    {
        ifstream enc(compressed, ios::in | ios::binary);
        if (!enc.is_open()) return -1;
        ofstream dec(uncompressed, ios::out | ios::binary);
        Node *tmp = this;
        int counter = 0;
        char buff;
        enc >> noskipws >> buff;
        bool flag;
        while (!enc.eof())
        {
            //cout << noskipws << "char " << char(buff & (1 << (7 - counter))) << endl;
            flag = (buff & (1 << (7 - counter)));
            if (flag) tmp = tmp->right;
            else tmp = tmp->left;
            if (tmp->left==nullptr && tmp->right==nullptr)
            {
                //if (tmp->ch == 13) cout << "new_line" << endl;
                //else cout << tmp->ch << endl;
                if (tmp->ch != 10)
                {
                    cout << noskipws << tmp->ch;
                    dec << noskipws << tmp->ch;
                }
                else
                {
                    cout << noskipws << "\r\n";
                    dec << noskipws << "\r\n";
                }
                tmp = this;
            }
            ++counter;
            if (counter == 8)
            {
                counter ^= counter;
                enc >> noskipws >> buff;
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
    //treeFile >> _key;
    //tab[10] = _key;
    //cout << "tab[new_line] = " << _key << endl;
    //--number;
    //treeFile >> _key;
    //tab[32] = _key;
    //cout << "tab[ ] = " << _key << endl;
    //--number;
    while(number != 0)
    {
        treeFile >> noskipws >> _ch >> _key;
        if (_ch == 13) _ch = 10;
        tab[_ch] = _key;
        cout << "tab[" << _ch << "] = " << _key << endl;
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
        char _ch = iter->first;
	    if (_ch > 31 && _ch < 127)
        {
	    	Node *tmp = new Node;
	    	tmp->ch = iter->first;
		    tmp->key = iter->second;
	    	tree.push_back(tmp);
	    }
        if (_ch == 10)
        {
            Node *tmp = new Node;
            tmp->ch = '\n';
            tmp->key = iter->second;
            tree.push_back(tmp);
        }
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
        //cout << "tree.size() = " << tree.size() << endl << "l = " << *l << " r = " << *r << endl;
    }
    Node *root = tree.front();
    for (int i = 10; i--;) char ch__ = treeFile.get();
    long int sizeOfFile = 0;
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
