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

using namespace std;
typedef complex<double> P;

// 道を表す。道は交差点を key にしてテーブルがひかれるので、開始側の交差点はこの
// struct の中には入っていない。(入れる必要が無い。)
struct Road {
    int id;               // 道の id
    int oppositeSect;     // intersects; 反対側の交差点
    int dir, oppositeDir; // どの方向に出ているか？　oppositeDir は反対側
    int len;              // length;
    int marker;           // marker までの距離。-1 は marker がないの意味。
    
    Road(int id, int s, int d1, int d2, int len) : id(id), oppositeSect(s), dir(d1), oppositeDir(d2), len(len), marker(-1) {
    }

    
    void setMarker(int m) {
        assert(0 < m && m < len);
        if (marker < 0) { marker = m; }
        else { marker = min(m, marker); }
    }

};

struct Graph {
    // inputs
    int ncp, nroad, ncm, confdist, startsect, endsect, startdir;
    set<int> choicePoints;
    vector<vector<Road> > roadMap; // sect -> road

    // answers
    int hareLength;
    int houndSearch;
    vector<int> resultRoute;

    Graph() : hareLength(0), houndSearch(0), roadMap(100) {}

    void simulate() {
        int currentSect = startsect;
        int currentDir = startdir;

        // 現在位置が goal に到達するまで続ける
        // 注意：　start と goal が同じ input がくることがあるが、いったん start しなければならない！
        // これは絶対ハマるポイント。絶対ハマるポイント。大事なことなので２回言いました。
        do {
//            cout << "CUR" << currentSect << ' ' << currentDir << endl;
            // ここが choicepoint かどうかで処理を分ける。
            if (choicePoints.count(currentSect)) {
                stepCheckPoint(currentSect, currentDir);
            } else {
                stepNotCheckPoint(currentSect, currentDir);
            }
        }
        while (currentSect != endsect);
    }

    // choicepoint でない交差点の場合、単に main road rule に従って進めば良い。
    void stepNotCheckPoint(int& currentSect, int& currentDir) {
        const vector<Road>& curRoads = roadMap[currentSect];
        int selectedMainRoadIdx = selectMainRoad(curRoads, currentSect, currentDir);
        assert(selectedMainRoadIdx >= 0);

        const Road& road = curRoads[selectedMainRoadIdx];

        hareLength += road.len;
        houndSearch += road.len;
        currentDir = normalizeDir(road.oppositeDir + 180);
        currentSect = road.oppositeSect;
        resultRoute.push_back(road.id);
    }

    // choice point の交差点の場合
    void stepCheckPoint(int& currentSect, int& currentDir) {
//        cout << "SCP" << currentSect << endl;
        // 使ってない road を覚えておく必要がある
        vector<Road> unselectedRoads(roadMap[currentSect]);

        // この交差点に来るときに使った道は、unselectedRoads からは外す。
        // このチェックは交差点に道路が入る角度で行っている。同じ角度で２つ以上の交差点はないことが保証されいているから。
        if (!resultRoute.empty()) {
            vector<Road>::iterator it;
            int d1 = normalizeDir(currentDir + 180);
            for (it = unselectedRoads.begin(); it != unselectedRoads.end(); ++it) {
                int d2 = normalizeDir(it->dir);
//                cout << d1 << ' ' << d2 << endl;
                if (d1 == d2) { break; }
            }
            assert(it != unselectedRoads.end());
            unselectedRoads.erase(it);
        }

        while (!unselectedRoads.empty()) {
            // main road rule ですすんでみる。
            int selectedMainRoadIdx = selectMainRoad(unselectedRoads, currentSect, currentDir);
            assert(selectedMainRoadIdx >= 0);
            const Road road = unselectedRoads[selectedMainRoadIdx];
            vector<int> tmpRoute;
            // すすんだあと、確認モードに入る。confirmation marker がみつからなければ false が帰ってくる。
            // みつかれば、確認モードでそれまでに進んだルートを加える。
            if (confirm(currentSect, currentDir, tmpRoute, road, confdist)) {
                resultRoute.insert(resultRoute.end(), tmpRoute.begin(), tmpRoute.end());
                return;
            } else {
                // confirmation marker が見つからなければ、帰って来た道からみた main road rule をたどる。
                currentDir = normalizeDir(road.dir + 180);
                vector<Road>::iterator it = unselectedRoads.begin() + selectedMainRoadIdx;
                unselectedRoads.erase(it);
            }
        }

        // ただしい道がみつからなかった。これはどこかがおかしい。Runtime Error を起こして知らせる。
        abort();
    }

    // confirmation marker を探すモード。
    bool confirm(int& currentSect, int& currentDir, vector<int>& routes, const Road& road, int restDist) {
//        cout << "CONFIRM : " << currentSect << ' ' << currentDir << endl;
        int markerLen = road.marker;
        // 
        if (markerLen < 0) { // この交差点から main road rule で進んだ道の１本目には confirmation marker がなかった。
            // もし道を全部進んでもまだ探索のための距離が残っているのならば次の交差点まで進んでみる。
            if (road.len < restDist) {
                int nextSect = road.oppositeSect;
                int nextDir = normalizeDir(road.oppositeDir + 180);
                const vector<Road>& nextRoads = roadMap[nextSect];
                if (nextRoads.size() >= 2 && !choicePoints.count(nextSect)) {
                    int selectedMainRoadIdx = selectMainRoad(nextRoads, nextSect, nextDir);
                    assert(selectedMainRoadIdx >= 0);
                    routes.push_back(road.id);
                    if (confirm(nextSect, nextDir, routes, nextRoads[selectedMainRoadIdx], restDist - road.len)) {
                        hareLength += road.len;
                        houndSearch += road.len;
                        currentSect = nextSect;
                        currentDir = nextDir;
                        return true;
                    } else {
                        houndSearch += road.len * 2;
                        return false;
                    }
                } else {
                    // いきどまり、or もう道がない or あたらしい choice point が見つかった。
                    // これは元の道にもどらねばならない。
                    // dead end, no more roads, or new choice point
                    houndSearch += road.len * 2;
                    return false;
                }
            } else {
                houndSearch += restDist * 2;
                return false;
            }
            
        } else if (markerLen <= restDist) { // 行こうとしている道にマーカーが見つかり、かつそれが探索空間内になる。
            // found.
            hareLength += road.len;
            houndSearch += road.len;
            currentSect = road.oppositeSect;
            currentDir = normalizeDir(road.oppositeDir + 180);
            routes.push_back(road.id);
            return true;
        } else { // 行こうとしている道にマーカーが見つかったが探索空間の外にあるので途中まで行ったら戻らねばならない。
            houndSearch += restDist * 2;
            return false;
        }
    }

    // curDir から見た main road を選択
    // main road が roads の何番目に格納されているかを返す
    int selectMainRoad(const vector<Road>& roads, int curSect, int curDir) {
        int selectedIdx = -1;
        int minDirDiff = 1000;
        for (int i = 0; i < roads.size(); ++i) {
            int dirDiff = normalizeDir(roads[i].dir - curDir);
            if ((abs(dirDiff) < abs(minDirDiff)) ||
                (abs(dirDiff) == abs(minDirDiff) && dirDiff > minDirDiff)) {
                selectedIdx = i;
                minDirDiff = dirDiff;
            }
        }

        return selectedIdx;
    }
    
    // (-180, 180] の範囲に dir を変換する。
    static int normalizeDir(int dir) {
        while (dir <= -180) { dir += 360; }
        while (dir > 180) { dir -= 360; }
        return dir;
    }
};

int main(void)
{
    int caseNo = 0;
    while (true) {
        Graph g;
        cin >> g.ncp >> g.nroad >> g.ncm >> g.confdist >> g.startsect >> g.endsect >> g.startdir;
        if (!(g.ncp || g.nroad || g.ncm || g.confdist || g.startsect || g.endsect || g.startdir)) {
            break;
        }
        --g.startsect, --g.endsect; // make graph 0-origin

        for (int i = 0; i < g.ncp; ++i) {
            int cp; cin >> cp;
            g.choicePoints.insert(cp - 1);
        }

        for (int i = 0; i < g.nroad; ++i) {
            int s1, s2, d1, d2, len;
            cin >> s1 >> s2 >> d1 >> d2 >> len;
            g.roadMap[s1 - 1].push_back(Road(i, s2 - 1, d1, d2, len));
            g.roadMap[s2 - 1].push_back(Road(i, s1 - 1, d2, d1, len));
        }

        for (int i = 0; i < g.ncm; ++i) {
            int s, r, l; cin >> s >> r >> l; --s, --r;
            for (int j = 0; j < g.roadMap.size(); ++j) {
                for (int k = 0; k < g.roadMap[j].size(); ++k) {
                    if (g.roadMap[j][k].id != r) { continue; }
                    
                    if (j == s) {
                        g.roadMap[j][k].setMarker(l);
                    } else {
                        g.roadMap[j][k].setMarker(g.roadMap[j][k].len - l);
                    }
                }
            }
        }

        g.simulate();
        
        cout << "Case " << (++caseNo) << ":" << endl;
        cout << "   Length of hare's route is " << g.hareLength << endl;
        cout << "   Length of hound's search is " << g.houndSearch << endl;
        cout << "   Route:";
        for (int i = 0; i < g.resultRoute.size(); ++i) {
            cout << ' ' << (g.resultRoute[i] + 1);
        }
        cout << endl << endl;
    }
    return 0;
}

