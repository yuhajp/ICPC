#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;
typedef vector<int>   vi;
typedef vector<vi>   vvi;
typedef vector<vvi> vvvi;

//
// 解法
//  あるマス (x, y) からゴールへの最短距離 d(x, y) は、i を方向として
//  d(x, y) = min max(d(x+dx[i], y+dy[i])+1,
//                    i 方向の扉が壊れている場合に、(y, x) から bfs して求めた最短距離);
//  なので、なるべく答が少ないところから dijkstra で求める。
//

enum Direction {
    NORTH, EAST, SOUTH, WEST
};

const int dy[] = { -1, 0, 1,  0 };
const int dx[] = {  0, 1, 0, -1 };
const int INF = INT_MAX / 2;

struct Cell {
    bool walls[4];
    Cell() { walls[0] = walls[1] = walls[2] = walls[3] = 0; }
};

typedef pair<int, int> Vertex;
typedef int Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef map<Vertex, Edges> Graph;

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

typedef map<Vertex, Weight> Potential;

int& wall(int y, int x, int dir, vvi& walls, int H, int W)
{
    static int dummy = 1;
    switch (dir) {
    case NORTH:
        if (y > 0)
            return walls[2*y-1][x];
        else
            return dummy;
    case EAST:
        if (x < W-1)
            return walls[2*y][x];
        else
            return dummy;
    case SOUTH:
        if (y < H-1)
            return walls[2*y+1][x];
        else
            return dummy;
    case WEST:
        if (x > 0)
            return walls[2*y][x-1];
        else
            return dummy;
    }
}

int bfs(int startY, int startX, vvi& walls, int H, int W)
{
    vvi d(H, vi(W, -1));
    d[startY][startX] = 0;

    queue<Vertex> Q;
    Q.push(Vertex(startY, startX));

    while (!Q.empty()) {
        Vertex v = Q.front(); Q.pop();
        for (int i = 0; i < 4; ++i) {
            int y = v.first;
            int x = v.second;
            if (wall(y, x, i, walls, H, W))
                continue;
            int yy = y + dy[i];
            int xx = x + dx[i];
            if (d[yy][xx] >= 0)
                continue;
            d[yy][xx] = d[y][x] + 1;
            Q.push(Vertex(yy, xx));
        }
    }

    if (d[H-1][W-1] < 0)
        return INF;
    else
        return d[H-1][W-1];
}

int solve(int H, int W, vvi& walls)
{
    // In advance, calculate the minimum distance 
    // when it turns out a wall is broken.
    vvvi minDistances(H, vvi(W, vi(4, -1)));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            for (int i = 0; i < 4; ++i) {
                if (wall(y, x, i, walls, H, W))
                    continue;
                wall(y, x, i, walls, H, W) = true;
                minDistances[y][x][i] = bfs(y, x, walls, H, W);
                wall(y, x, i, walls, H, W) = false;
            }
        }
    }

    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;
    Q.push(Edge(Vertex(H-1, W-1), 0));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        if (p.first == 0 && p.second == 0) {
            if (curr >= INF)
                return -1;
            else
                return curr;
        }
        if (pot.count(p)) { continue; } // already visited.
        pot[p] = curr;

        for (int i = 0; i < 4; ++i) {
            if (wall(p.first, p.second, i, walls, H, W))
                continue;
            int yy = p.first + dy[i];
            int xx = p.second + dx[i];
            int cost = curr + 1;
            if (cost < minDistances[yy][xx][(i+2) % 4])
                cost = minDistances[yy][xx][(i+2) % 4];

            if (pot.count(Vertex(yy, xx))) continue;
            Q.push(Edge(Vertex(yy, xx), cost));
        }
    }

    return -1;
}

int main(void)
{
    for (int H, W; cin >> H >> W, (H || W); ) {
        vector<vector<int> > walls(2*H, W);
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W-1; ++w)
                cin >> walls[2*h][w];
            if (h == H-1) break;
            for (int w = 0; w < W; ++w)
                cin >> walls[2*h+1][w];
        }

        cout << solve(H, W, walls) << endl;
    }

    return 0;
}
