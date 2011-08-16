#include <iostream>
#include <set>
#include <map>
#include <string>
#include <cassert>

using namespace std;

typedef map<pair<int, int>, int> structures_t;
int freshStno = 1;
structures_t structures;

struct Node {
    Node* left;
    Node* right;
    int stNo;
    set<int> sts;
    double similarity;
  
    Node(Node* left = NULL, Node* right = NULL) : 
        left(left), right(right)
    {
        if (left == NULL) {
            assert (right == NULL);
            stNo = 0;
            sts.insert(0);
            similarity = 0;
        } else {
            assert (right != NULL);
            pair<int, int> p = make_pair(min(left->stNo, right->stNo), max(left->stNo, right->stNo));
            if (structures.count(p)) {
                stNo = structures[p];
            } else {
                stNo = structures[p] = freshStno++;
            }
      
            sts.insert(left->sts.begin(), left->sts.end());
            sts.insert(right->sts.begin(), right->sts.end());
            int n = sts.size();
            int m = 0;
            for (set<int>::iterator it = sts.begin(); it != sts.end(); ++it) {
                if (left->sts.find(*it) != left->sts.end() &&
                    right->sts.find(*it) != right->sts.end()) {
                    ++m;
                }
            }

            similarity = double(m) / n;
      
            sts.insert(stNo);
        }
    }
  
    friend int compare(Node* left, Node* right)
    {
        if (left->similarity < right->similarity) {
            return 1;
        }
        if (left->similarity > right->similarity) {
            return -1;
        }

        if (left->left == NULL) {
            return 0;
        }

        Node* left_max;
        Node* left_min;
        Node* right_max;
        Node* right_min;

        if (compare(left->left, left->right) == -1) {
            // if (left->left->similarity > left->right->similarity) {
            left_max = left->left;
            left_min = left->right;
        } else {
            left_max = left->right;
            left_min = left->left;
        }

        if (compare(right->left, right->right) == -1) {
            // if (right->left->similarity > right->right->similarity) {
            right_max = right->left;
            right_min = right->right;
        } else {
            right_max = right->right;
            right_min = right->left;
        }

        int c = compare(left_min, right_min);
        if (c != 0) { return c; }

        return compare(left_max, right_max);
    }

    string unparse(bool isLeft) {
        if (left == NULL) { return "x"; }
        Node* pLeft;
        Node* pRight;

        int p = compare(left, right);
        if (p == 1) {
            pLeft = isLeft ? left : right;
            pRight = isLeft ? right : left;
        } else {
            pLeft = isLeft ? right : left;
            pRight = isLeft ? left : right;
        }

        return "(" + pLeft->unparse(true) + " " + pRight->unparse(false) + ")";
    }
  
  
    ~Node() {
        delete left;
        delete right;
    }
};



Node* read_tree()
{
    char c; cin >> c;
    if (c == 'x') {
        return new Node();
    } else if (c == '0') {
        return NULL;
    } else if (c == '(') {
        Node* left = read_tree();
        Node* right = read_tree();
        Node* tree = new Node(left, right);
        cin >> c; // skip ')'
        return tree;
    }
}

int main(void)
{
    Node* tree;
    while ((tree = read_tree()) != NULL) {
        cout << tree->unparse(true) << endl;
        delete tree;
    }
}
