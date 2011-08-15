#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    for (int M; cin >> M, M; ) {
        vector<int> nexts(M + 1);
        vector<int> prevs(M + 1);
        for (int I, J; cin >> I >> J, (I || J); ) {
            if (I == J) { continue; }

            if (J == 0 && prevs[I] == 0) { continue; }
            
            // skip if I is up on J.
            {
                bool IonJ = false;
                int cur = J;
                while (cur) {
                    if (cur == I) { IonJ = true; break; }
                    cur = nexts[cur];
                }

                if (IonJ) { continue; }
            }

            // make separate
            {
                int cur = I;
                while (cur) {
                    nexts[prevs[cur]] = 0;
                    prevs[cur] = 0;
                    cur = nexts[cur];
                }
            }

            int topJ = J;
            while (nexts[topJ]) {
                topJ = nexts[topJ];
            }
            if (topJ != 0) {
                nexts[topJ] = I;
            }
            prevs[I] = topJ;
        }

        // print
        vector<int> heights;
        for (int i = 1; i <= M; ++i) {
            if (prevs[i]) { continue; }
            int cur = i;
            int height = 0;
            while (cur) {
                ++height;
                cur = nexts[cur];
            }
            heights.push_back(height);
        }
        sort(heights.begin(), heights.end());
        for (int i = 0; i < heights.size(); ++i) {
            cout << heights[i] << endl;
        }
        cout << "end" << endl;
    }
}
