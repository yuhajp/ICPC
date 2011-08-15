#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct Pos
{
    int x, y, z;
    
    Pos(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    void rotate(int dir) {
        switch (dir) {
        case 0:
            *this = Pos(x, -z, y); return;
        case 1:
            *this = Pos(z, y, -x); return;
        case 2:
            *this = Pos(-y, x, z); return;
        }
    }

    friend bool operator<(const Pos& lhs, const Pos& rhs)
    {
        if (lhs.x != rhs.x) { return lhs.x < rhs.x; }
        if (lhs.y != rhs.y) { return lhs.y < rhs.y; }
        return lhs.z < rhs.z;
    }

    friend bool operator!=(const Pos& lhs, const Pos& rhs)
    {
        return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
    }

    friend bool operator==(const Pos& lhs, const Pos& rhs)
    {
        return !(lhs != rhs);
    }
};

struct Key
{
    typedef vector<pair<Pos, Pos> > bars_t;
    bars_t bars;

    void add(const Pos& a, const Pos& b) {
        bars.push_back(make_pair(a, b));
    }

    void normalize() {
        int minX = 10000, minY = 10000, minZ = 10000;
        for (int i = 0; i < bars.size(); ++i) {
            minX = std::min(minX, std::min(bars[i].first.x, bars[i].second.x));
            minY = std::min(minY, std::min(bars[i].first.y, bars[i].second.y));
            minZ = std::min(minZ, std::min(bars[i].first.z, bars[i].second.z));
        }
        for (int i = 0; i < bars.size(); ++i) {
            bars[i].first.x -= minX; bars[i].second.x -= minX;
            bars[i].first.y -= minY; bars[i].second.y -= minY;
            bars[i].first.z -= minZ; bars[i].second.z -= minZ;
            if (bars[i].second < bars[i].first) {
                swap(bars[i].first, bars[i].second);
            }
        }
        sort(bars.begin(), bars.end());
    }

    void rotate(int dir) {
        for (int i = 0; i < bars.size(); ++i) {
            bars[i].first.rotate(dir);
            bars[i].second.rotate(dir);
        }
    }

    void rotate(int dirs[3], int times[3]) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < times[i]; ++j) {
                rotate(dirs[i]);
            }
        }
    }
};

bool compare_sub(const Key& a, const Key& b)
{
    if (a.bars.size() != b.bars.size()) { return false; }
    for (int i = 0; i < a.bars.size(); ++i) {
        if (a.bars[i] != b.bars[i]) { return false; }
    }
    return true;
}

bool compare(Key a, Key b)
{
    a.normalize();

    // XYZ, XZY, YXZ, YZX, ZXY, ZYX
    // 回転させる順番がかわると一般には同じ様にならないので、全ての順番を試す。
    int dirs[6][3] = {
        { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
    };

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4 * 4 * 4; ++j) {
            Key tgt = b;
            // 各軸を何回回転させるか？
            int times[3] = { j % 4, (j / 4) % 4, (j / 16) % 4};
            tgt.rotate(dirs[i], times);

            tgt.normalize();
            if (compare_sub(a, tgt)) { return true; }
        }
    }
    
    return false;
}

bool create_key(Key& key)
{
    int N; cin >> N;
    if (N == 0) { return false; }

    Pos p;
    map<int, Pos> ps;
    for (int i = 0; i < N; ++i) {
        string str; cin >> str;
        if (str == "+x") {
            Pos q = Pos(p.x + 1, p.y, p.z);
            key.add(p, q);
            p = q;
        } else if (str == "+y") {
            Pos q = Pos(p.x, p.y + 1, p.z);
            key.add(p, q);
            p = q;
        } else if (str == "+z") {
            Pos q = Pos(p.x, p.y, p.z + 1);
            key.add(p, q);
            p = q;
        } else if (str == "-x") {
            Pos q = Pos(p.x - 1, p.y, p.z);
            key.add(p, q);
            p = q;
        } else if (str == "-y") {
            Pos q = Pos(p.x, p.y - 1, p.z);
            key.add(p, q);
            p = q;
        } else if (str == "-z") {
            Pos q = Pos(p.x, p.y, p.z - 1);
            key.add(p, q);
            p = q;
        } else {
            int k = atoi(str.c_str());
            if (ps.count(k)) {
                p = ps[k];
            } else {
                ps[k] = p;
            }
        }
    }

    return true;
}

int main(void)
{
    while (true) {
        Key a, b;
        if (!create_key(a)) { break; }
        if (!create_key(b)) { throw "shouldn't happen"; }
        cout << (compare(a, b) ? "SAME" : "DIFFERENT") << endl;
    }
    return 0;
}
