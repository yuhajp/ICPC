#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

typedef pair<int, int> pi;

struct Rect {
    int top, left, bottom, right;
    Rect(int top, int left, int bottom, int right) :
        top(top), left(left), bottom(bottom), right(right)
    {}
};

struct Field {
    int H, W;
    vvi vs;
    vvi sums;

    Field(int H, int W, const vvi& vs) :
        H(H), W(W), vs(vs), sums(H, vi(W))
    {
        makeSum();
    }

    int sum(const Rect& r) const {
        int s = sums[r.bottom-1][r.right-1];
        if (r.left-1 >= 0)
            s -= sums[r.bottom-1][r.left-1];
        if (r.top-1 >= 0)
            s -= sums[r.top-1][r.right-1];
        if (r.left-1 >= 0 && r.top-1 >= 0)
            s += sums[r.top-1][r.left-1];
        return s;
    }

private:
    void makeSum() {
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                sums[i][j] = vs[i][j];
                if (i-1 >= 0)
                    sums[i][j] += sums[i-1][j];
                if (j-1 >= 0)
                    sums[i][j] += sums[i][j-1];
                if (i-1 >= 0 && j-1 >= 0)
                    sums[i][j] -= sums[i-1][j-1];                
            }
        }

        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                if (vs[i][j] != sum(Rect(i, j, i + 1, j + 1)))
                    abort();
    }
};

struct Cache {
    int H, W;
    pi cache[33][33][33][33];
    bool exists[33][33][33][33];

    Cache(int H, int W) : 
        H(H), W(W)
    {
        memset(exists, 0, sizeof(exists));
    }

    int count(const Rect& rect)
    {
        return exists[rect.top][rect.left][rect.bottom][rect.right];
    }

    pi& get(const Rect& rect) {
        return cache[rect.top][rect.left][rect.bottom][rect.right];
    }

    void set(const Rect& rect, const pi& p) {
        cache[rect.top][rect.left][rect.bottom][rect.right] = p;
        exists[rect.top][rect.left][rect.bottom][rect.right] = true;
    }

};

bool iter(Rect rect, const Field& field, int S, Cache& cache)
{
    if (S < 0)
        return false;
    if (cache.count(rect))
        return cache.get(rect).first >= 0;
    
    pair<int, int> mx = make_pair(1, S);
    
    // The case that the rect is not cut.
    
    // The case that the rect is cut vertically
    for (int i = rect.left + 1; i < rect.right; ++i) {
        Rect left(rect.top, rect.left, rect.bottom, i);
        Rect right(rect.top, i, rect.bottom, rect.right);
        if (!iter(left, field, S - field.sum(right), cache) ||
            !iter(right, field, S - field.sum(left), cache))
            continue;

        pair<int, int> m = 
            make_pair(cache.get(left).first + cache.get(right).first,
                      min(cache.get(left).second, cache.get(right).second));
        if (mx < m)
            mx = m;
    }

    for (int i = rect.top + 1; i < rect.bottom; ++i) {
        Rect upper(rect.top, rect.left, i, rect.right);
        Rect lower(i, rect.left, rect.bottom, rect.right);
        if (!iter(upper, field, S - field.sum(lower), cache) ||
            !iter(lower, field, S - field.sum(upper), cache))
            continue;

        pair<int, int> m = 
            make_pair(cache.get(upper).first + cache.get(lower).first,
                      min(cache.get(upper).second, cache.get(lower).second));
        if (mx < m)
            mx = m;        
    }

    cache.set(rect, mx);
    return mx.first >= 0;
}

pair<int, int> solve(const Field& field, int S)
{
    Cache* cache = new Cache(field.H, field.W);
    Rect r(0, 0, field.H, field.W);

    iter(r, field, S, *cache);

    return cache->get(r);
}

int main(void)
{
    for (int H, W, S; cin >> H >> W >> S, (H || W || S); ) {
        vvi vs(H, vi(W));
        for (int h = 0; h < H; ++h)
            for (int w = 0; w < W; ++w)
                cin >> vs[h][w];

        Field field(H, W, vs);
        pair<int, int> ps = solve(field, S);
        cout << ps.first << ' ' << ps.second << endl;
    }
}
