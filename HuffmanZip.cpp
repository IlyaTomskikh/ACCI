#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
using namespace std;

vector<bool> hufCode;
map<char, vector<bool> > trueTab;
string compressed = "compressedText.txt",
uncompressed = "uncompressedText.txt";
ofstream treeFile("tree.txt", ios::out | ios::binary);
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
        //ch = '\n';
        key = l->key + r->key;
    }

    void tabCreater()
    {
        if (left != nullptr)
        {
            hufCode.push_back(0);
            left->tabCreater();
        }
        if (right != nullptr)
        {
            hufCode.push_back(1);
            right->tabCreater();
        }
        if (left == nullptr && right == nullptr)
        {
            cout << "leaf: " << *this << endl;
            trueTab[ch] = hufCode;
        }
        hufCode.pop_back();
    }

    int zip(string fileName)
    {
        ifstream input(fileName, ios::out | ios::binary);
        if (!input.is_open()) return -1;
        ofstream output(compressed, ios::out | ios::binary);
        int counter = 0;
        char buff = 0;
        while(!input.eof())
        {
            char _ch;
            input >> noskipws >> _ch;
            //cout << "->" << _ch << "<-" << endl;
	        if (_ch > 31 && _ch < 127 || _ch == 10)
            {
                //if (_ch == 13) cout << "new_line" << endl;
                vector<bool> res = trueTab[_ch];
                for (int j = 0; j < res.size(); ++j)
                {
                	buff |= (res[j] << (7 - counter));
                	++counter;
                	if (counter == 8)
                	{
                    	counter ^= counter;
                    	output << buff;
                        //cout << buff << endl;
                    	buff ^= buff;
                    }
                }
	        }
        }
        if (counter >= 0 && counter < 8) output << buff;
        input.close();
        output.close();
        return 1;
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

    friend ostream & operator<< (ostream &out, const Node &that)
    {
        if (that.ch == 10) return out << "[new_line] = " << that.key << endl;
        else return out << '[' << that.ch << "] = " << that.key << endl;
    }
};

int fileSize(string add)
{
    ifstream mySource;
    mySource.open(add, ios_base::binary);
    mySource.seekg(0,ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}

struct check
{
    bool operator() (const Node *l, const Node *r) const
    {
        return l->key < r->key;
    }
};

int main()
{
    string fileName = "originalText.txt";
    ifstream input(fileName, ios::out | ios::binary);
    map<char, int> tab;
    while(!input.eof())
    {
        char _ch;
        input >> noskipws >> _ch;
        if (_ch > 31 && _ch < 127) tab[_ch]++;
        if (_ch = 13) tab[10]++;
    }
    cout << tab.size() << endl;
    treeFile << tab.size();
    list<Node*> tree;
    for (map<char, int>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
        Node *tmp = new Node;
        char _ch = iter->first;
	    if (_ch != 10)
        {
	    	tmp->ch = iter->first;
		    tmp->key = iter->second;
            treeFile << tmp->ch << tmp->key;
	    	tree.push_back(tmp);
	    }
        else 
        {
            tmp->ch = '\n';
            tmp->key = iter->second;
            treeFile << '\r' << tmp->key;
            tree.push_back(tmp);
        }
        cout << noskipws << "ch = " << tmp->ch << ", key = " << tmp->key << endl; 
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
        cout << noskipws << "tree.size() = " << tree.size() << endl << "l = " << *l << " r = " << *r << endl;
    }
    Node *root = tree.front();
    root->tabCreater();
    input.close();
    int test = root->zip(fileName);
    treeFile << "fileSizeIs" << fileSize(fileName);
    cout << "size of file = " << fileSize(fileName) << " bytes" << endl;
    treeFile.close();
    if (test == -1)
    {
        cout << "Couldn't open file " << fileName << endl;
        return 666;
    }
    cout << "File "<< fileName << " was compressed and rewritten to file " << compressed << endl;
    return 777;
}
