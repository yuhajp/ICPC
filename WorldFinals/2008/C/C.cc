#include<iostream>
#include<cmath>
#include<cstdio>
#include<string>
#include<complex>
#include<vector>

using namespace std;

typedef complex<double> cd;

typedef struct shaft {
       cd c;
       double r;
       bool cc;
} SHAFT;

typedef struct edge {
       int srcIndex;
       int dstIndex;
       double srcArg;
       double dstArg;
       cd srcPoint;
       cd dstPoint;
       double length;
} EDGE;

#define INF 1e+10
#define EPS 1e-8
#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl
#define MY_PI acos(-1.0)

/** global **/

int n;
int srcIndex;
int dstIndex;
double limit;
SHAFT shafts[30];

void printShaft(SHAFT &shaft) {
       cout << shaft.c << endl;
       cout << shaft.r << endl;
       cout << shaft.cc << endl;
}

cd myarg(double arg) {
       return cd(cos(arg), sin(arg));
}

/*
       0 < t < 1
       | p1 + t (p2 - p1) - c | = r
       | p1 - c + t (p2 - p1) | = r
       | p + tv | = r
       (p+tv)conj(p+tv) = r*r
       p conj(p) + t*t v conj(v) + {p conj(v) + conj(p) v} t - r*r
*/

bool conflictBeltAndShaft(cd p1, cd p2, int skipIndex1, int skipIndex2) {
       for(int i = 0; i < n; i++) {
               if(i == skipIndex1)continue;
               if(i == skipIndex2)continue;
               cd p = p1 - shafts[i].c;
               cd v = p2 - p1;
               double a2 = norm(v);
               double a1 = real(p*conj(v) + conj(p)*v);
               double a0 = norm(p) - shafts[i].r*shafts[i].r;
               double d = a1*a1 - 4.0*a2*a0;
               if(d < 0.0)continue;
               double t1 = (-a1 + sqrt(d)) / (2.0*a2);
               if(0.0 <= t1 && t1 <= 1.0)return true;
               double t2 = (-a1 - sqrt(d)) / (2.0*a2);
               if(0.0 <= t2 && t2 <= 1.0)return true;
       }
       return false;
}

vector<EDGE> makeEdges(int n, const SHAFT* shafts, double limit) {
       vector<EDGE> output;
       for(int i = 0; i < n; i++) {
               SHAFT shaft1 = shafts[i];
               cd c1 = shaft1.c;
               double r1 = shaft1.r;
               bool cc1 = shaft1.cc;
               for(int j = 0; j < n; j++) {
                       if(i == j)continue;
                       SHAFT shaft2 = shafts[j];
                       cd c2 = shaft2.c;
                       double r2 = shaft2.r;
                       bool cc2 = shaft2.cc;

                       cd c = c1 - c2;
                       double r = abs(c);
                       double phi = arg(c);

                       EDGE edge;
                       edge.srcIndex = i;
                       edge.dstIndex = j;

                       if(cc1 && cc2) {
                               double theta = phi + acos((r2-r1)/r);
                               edge.srcArg = theta;
                               edge.dstArg = theta;
                       } else if(!cc1 && !cc2) {
                               double theta = phi - acos((r2-r1)/r);
                               edge.srcArg = theta;
                               edge.dstArg = theta;
                       } else if(cc1 && !cc2) {
                               double theta = phi + acos((-r2-r1)/r);
                               edge.srcArg = theta;
                               edge.dstArg = theta + MY_PI;
                       } else { /* !cc1 && cc2 */
                               double theta = phi - acos((-r2-r1)/r);
                               edge.srcArg = theta;
                               edge.dstArg = theta + MY_PI;
                       }
                       edge.srcPoint = c1 + r1 * myarg(edge.srcArg);
                       edge.dstPoint = c2 + r2 * myarg(edge.dstArg);
                       edge.length = abs(edge.srcPoint - edge.dstPoint);
                       if(edge.length > limit)continue;
                       if(conflictBeltAndShaft(edge.srcPoint, edge.dstPoint, i, j))continue;

                       output.push_back(edge);
               }
       }
       return output;
}

/*
       p1 + t1*v1 = p2 + t2*v2
       t1*v1 = (p2 - p1) + t2*v2
       t1*v1/v2 = (p2 - p1)/v2 + t2
       t1*imag(v1/v2) = imag((p2-p1)/v2)
       t1 = imag((p2-p1)/v2) / imag(v1/v2)

*/
bool crossLines(cd p11, cd p12, cd p21, cd p22) {
       cd p1 = p11;
       cd v1 = p12 - p11;
       cd p2 = p21;
       cd v2 = p22 - p21;
       if(abs(imag(v1/v2)) < EPS)return false;
       double t1 = imag((p2-p1)/v2) / imag(v1/v2);
       double t2 = imag((p1-p2)/v1) / imag(v2/v1);
       return 0.0 <= t1 && t1 <= 1.0 && 0.0 <= t2 && t2 <= 1.0;
}

bool conflictBelts(vector<EDGE>& history, EDGE& newEdge) {
       for(vector<EDGE>::iterator edge = history.begin(); edge != history.end(); edge++) {
               if(crossLines(edge->srcPoint, edge->dstPoint, newEdge.srcPoint, newEdge.dstPoint))return true;
       }
       return false;
}

double rotateLength(double arg1, double arg2, double r, bool cc) {
       if(cc) {
               double output = arg2 - arg1 - 2.0 * MY_PI;
               while(output < 0.0)output += 2.0 * MY_PI;
               return r*output;
       } else {
               double output = arg1 - arg2 - 2.0 * MY_PI;
               while(output < 0.0)output += 2.0 * MY_PI;
               return r*output;
       }
}

double dfs(vector<EDGE>& edges, vector<EDGE>& history, bool* visit, double length, double answer) {
       if(length > answer) {
               return answer;
       }
       EDGE last = history[history.size() - 1];
       int lastShaftIndex = last.dstIndex;
       if(lastShaftIndex == dstIndex) {
               return length;
       }
       for(vector<EDGE>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
               if(edge->srcIndex != lastShaftIndex)continue;
               if(visit[edge->dstIndex])continue;
               if(conflictBelts(history, *edge))continue;
               history.push_back(*edge);
               visit[edge->dstIndex] = true;
               double lengthAdd = rotateLength(last.dstArg, edge->srcArg, shafts[lastShaftIndex].r, shafts[lastShaftIndex].cc) + edge->length;
               answer = dfs(edges, history, visit, length+lengthAdd, answer);
               history.pop_back();
               visit[edge->dstIndex] = false;
       }
       return answer;
}

double solve() {
       vector<EDGE> edges = makeEdges(n, shafts, limit);
       double answer = INF;
       bool visit[n];
       for(int i = 0; i < n; i++)visit[i] = false;
       vector<EDGE> history;
       visit[srcIndex] = true;
       for(vector<EDGE>::iterator edge = edges.begin(); edge != edges.end(); edge++) {
               if(edge->srcIndex != srcIndex)continue;

               history.push_back(*edge);
               visit[edge->dstIndex] = true;
               answer = dfs(edges, history, visit, edge->length, answer);
               history.pop_back();
               visit[edge->dstIndex] = false;
       }

       return answer;
}

int main(void) {
       int caseNumber;
       int sum = 0;
       for(caseNumber=1; true; caseNumber++) {
//              int n;
               cin >> n;
               sum += n;
               if(n == 0)break;

               for(int i = 0; i < n; i++) {
                       double x, y;
                       cin >> x >> y;
                       shafts[i].c = cd(x, y);
                       cin >> shafts[i].r;
                       string s;
                       cin >> s;
                       shafts[i].cc = (s == "CC");
//                      printShaft(shafts[i]);
               }
               cin >> srcIndex >> dstIndex >> limit;
               double answer = solve();
               if(answer < INF) {
                       /*
                       answer *= 100.0;
                       answer +=0.5;
                       answer = floor(answer);
                       answer /= 100.0;
                       */
                       printf("Case %d: length = %f\n", caseNumber, answer);
               } else {
                       printf("Case %d: Cannot reach destination shaft\n", caseNumber);
               }
       }
       /*
       if(caseNumber == 28) {
               throw "my exception";
       }
       if(sum > 235) {
               throw "my exception";
       }
       */
}
