// author: mayah
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <complex>
#include <cmath>
#include <cassert>

#include <fstream>

using namespace std;
typedef complex<double> P;

// ハフマン木を表す。
struct TNode {
    TNode* parent;
    TNode* left;
    TNode* right;

    int val;
    
    TNode(TNode* parent = NULL) : parent(parent), left(NULL), right(NULL), val(0) {}
    ~TNode() { delete left; delete right; }

    bool isLeft() {
        if (!parent) { return false; }
        return parent->left == this;
    }
    
    TNode* makeLeft() {
        if (left) { return left; }
        else { return left = new TNode(this); }
    }

    TNode* makeRight() {
        if (right) { return right; }
        else { return right = new TNode(this); }
    }
};

// 探索メソッド
int iter(int ith, vector<TNode*> arranged)
{
    // 全てに確率が割り当てられた場合
    if (ith >= arranged.size()) { return 1; }

    // そうでない場合
    TNode* node = arranged[ith];
    if (node->isLeft()) {
        // 左の子であれば、親と右の子から一瞬で求まる。
        int pv = node->parent->val;
        int rv = node->parent->right->val;
        int v = pv - rv;
        // 0 だったり、広義単調減少でなかったりしたらすぐに返る。
        if (v > rv) { return 0; }
        if (v == 0) { return 0; }
        node->val = v;
        return iter(ith + 1, arranged);
    } else { // right.
        // 右の子であれば、広義単調減少になるように値をセットして探索する。
        int sum = 0;
        int minVal = min(node->parent->val - 1, arranged[ith - 1]->val);
        for (int v = minVal; v > 0; --v) {
            node->val = v;
            sum += iter(ith + 1, arranged);
        }
        return sum;
    }
}

int solve(const vector<string>& str)
{
    // 木を構築
    TNode* root = new TNode();
    for (int i = 0; i < str.size(); ++i) {
        TNode* n = root;
        for (int j = 0; j < str[i].length(); ++j) {
            if (str[i][j] == '0') {
                n = n->makeLeft();
            } else {
                n = n->makeRight();
            }
        }
    }

    // BFS 順で並べる。ただし、枝は逆順。
    vector<TNode*> arranged;
    queue<TNode*> Q;
    Q.push(root);
    while (!Q.empty()) {
        TNode* n = Q.front(); Q.pop();
        arranged.push_back(n);

        if (n->right) { Q.push(n->right); }
        if (n->left) { Q.push(n->left); }
    }

    // root の確率は当然 100。あとは探索する。
    arranged[0]->val = 100;
    return iter(1, arranged);
}

int main(void)
{
    int num = 0;
    bool ok = false;
    for (int N; cin >> N, N; ) {
        ok = true;
        vector<string> str(N);
        for (int i = 0; i < N; ++i) {
            cin >> str[i];
        }

        cout << "Case " << (++num) << ": " << solve(str) << endl;
    }

    if (!ok) {
        abort();
    }
    
    return 0;
}
