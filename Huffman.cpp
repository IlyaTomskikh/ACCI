#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
using namespace std;

vector<bool> hufCode;
map<char, vector<bool> > trueTab;

class Node
{
    public:
    int key;
    char ch;
    Node *left;
    Node *right;

    Node()
    {
        left = right = nullptr;
    }
    
    Node(Node *l, Node *r) 
	{
        left =  l;
        right = r;
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
        if (left == nullptr && right == nullptr) trueTab[ch] = hufCode;
        hufCode.pop_back();
    }

    void zip(const char *fileName)
    {
        ifstream input(fileName, ios::out | ios::binary);
        ofstream output("compressedText.txt", ios::out | ios::binary);
        int counter = 0;
        char buff = 0;
        while(!input.eof())
        {
            char sym = input.get();
            vector<bool> res = trueTab[sym];
            for (int j = 0; j < res.size(); ++j)
            {
                buff |= (res[j] << (7 - counter));
                ++counter;
                if (counter == 8)
                {
                    counter = 0;
                    output << buff;
                    buff = 0;
                }
            }
        }
        input.close();
        output.close();
    }

    void unzip()
    {
        ifstream enc("compressedText.txt", ios::in | ios::binary);
        ofstream dec("uncomressedText.txt", ios::out | ios::binary);
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
                counter = 0;
                buff = enc.get();
            }
        }
        enc.close();
        dec.close();
    }

};

struct check
{
    bool operator() (const Node *l, const Node *r) const
    {
        return l->key < r->key;
    }
};

int main(int argc, char *argv[])
{
    const char *fileName = "originalText.txt";
    ifstream input(fileName, ios::out | ios::binary);
    map<char, int> tab;
    while(!input.eof())
    {
        char sym = input.get();
        tab[sym]++;
    }
    list<Node*> tree;
    for (map<char, int>::iterator iter = tab.begin(); iter != tab.end(); ++iter)
    {
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
    root->tabCreater();
    input.close();
    root->zip(fileName);
    root->unzip();
    return 0;
}
