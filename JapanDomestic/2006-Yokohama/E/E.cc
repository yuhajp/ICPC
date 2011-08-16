#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int ONE_MILLION = 1E+6;

struct Tree {
    int rep;
    char val;
    vector<Tree> children;

    Tree() : rep(1), val('-') {
    }
    
    Tree(int rep) : rep(rep), val('-') {
    }

    Tree(int rep, char val) : rep(rep), val(val) {
    }

    void print() {
        cout << "*" << rep << endl;
        if (val != '-') { cout << val << endl;}
        else {
            for (int i = 0; i < children.size(); ++i) {
                children[i].print();
            }
        }
        cout << "---" << endl;
    }

    int unitLength() const {
        int num = 0;
        if (val != '-') { return 1; }
        for (int i = 0; i < children.size(); ++i) {
            num += children[i].length();
        }
        if (num > ONE_MILLION) {
            return ONE_MILLION + 1;
        } else {
            return num;
        }
    }

    int length() const {
        int l = rep * unitLength();
        return (l > ONE_MILLION) ? ONE_MILLION + 1 : l;
    }

    char nth(int n) {
        //cout << "length = " << length() << endl;
        if (n >= length()) { return '0'; }

        int ul = unitLength();
        n %= ul;

        if (val != '-') {
            if (n < rep) { return val; }
            else { return '0'; } // SEARCH FAILED.
        }
        
        for (int i = 0; i < children.size(); ++i) {
            int s = children[i].length();
            if (n < s) {
                return children[i].nth(n);
            } else {
                n -= s;
            }
        }

        return '0';
    }
};

int parse(Tree& tree, const string& str, int& pos)
{
    //cout << "parsing : " << str[pos] << endl;
    while (pos < str.length()) {
        //cout << "P -- " << str[pos] << endl;
        int rep = 0;
        while ('0' <= str[pos] && str[pos] <= '9') {
            rep = rep * 10 + (str[pos] - '0');
            ++pos;
        }
        // cout << "rep = " << rep << endl;
        if (rep == 0) { rep = 1; }
        //cout << "Q -- " << str[pos] << endl;
        if (str[pos] == '(') {
            Tree child = Tree(rep);
            ++pos;
            parse(child, str, pos);
            tree.children.push_back(child);
        } else if (str[pos] == ')') {
            ++pos;
            return pos;
        } else {
            Tree child = Tree(rep, str[pos]);
            tree.children.push_back(child);
            ++pos;
        }
    }


    return pos;
}

int main(void)
{
    string str; int nth;
    while (cin >> str >> nth, !(str == "0" && nth == 0)) {
        // cout << str << ' ' << nth << endl;
        Tree tree;
        int pos = 0;
        if (parse(tree, str, pos) == str.length()) {
            // tree.print();
            cout << tree.nth(nth) << endl;
        } else {
            cout << pos << endl;
            abort(); // shouldn't happen
        }
    }

    return 0;
}
