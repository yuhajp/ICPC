#include<iostream>
#include<vector>
#include<algorithm>
#include<set>

using namespace std;


set<int> visit;
set<int> breakable;
vector<pair<int, int> > requestSizeAndCount;

int wMax;
int hMax;

int generateStateIndex(int w, int h, int* counts) {
    int output = 0;
    int multiply = 1;
    
    output += (multiply * (w-1));
    multiply *= wMax;
    
    output += (multiply * (h-1));
    multiply *= hMax;
    
    for (int i = 0; i < requestSizeAndCount.size(); i++) {
        output += (multiply * counts[i]);
        multiply *= (requestSizeAndCount[i].second + 1);
    }
    return output;
}

int getOnePieceArea(int* counts) {
    int area = -1;
    for (int i = 0; i < requestSizeAndCount.size(); i++) {
        if (counts[i] > 1) {
            return -1;
        } else if (counts[i] == 1) {
            if (area > 0) {
                return -1;
            } else {
                area = requestSizeAndCount[i].first;
            }
        }
    }
    return area;
}

bool isOnePieceFinish(int w, int h, int* counts) {
    return getOnePieceArea(counts) == w*h;
}

bool assignCountsAndDfs(int w1, int h1, int a1, int w2, int h2, int a2, int* counts, int* counts1, int* counts2, int index);
bool dfs(int w, int h, int* counts);

bool assignCountsAndDfs(int w1, int h1, int a1, int w2, int h2, int a2, int* counts, int* counts1, int* counts2, int index) {
    if (a1 < 0 || a2 < 0) {
        return false;
    }
    if (index == requestSizeAndCount.size()) {
        return dfs(w1, h1, counts1) && dfs(w2, h2, counts2);
    }
    
    for (int i = 0; i <= counts[index]; i++) {
        counts1[index] = i;
        counts2[index] = counts[index] - i;
        
        if (assignCountsAndDfs(w1, h1, a1 - counts1[index]*requestSizeAndCount[index].first, w2, h2, a2 - counts2[index]*requestSizeAndCount[index].first, counts, counts1, counts2, index+1)) {
            return true;
        }
    }
    return false;
}


bool dfs(int w, int h, int* counts) {
    
    if (h > w) {
        return dfs(h, w, counts);
    }
    
    int index = generateStateIndex(w, h, counts);
    if (visit.find(index) != visit.end()) {
        return breakable.find(index) != breakable.end();
    }
    
    visit.insert(index);
    if (isOnePieceFinish(w, h, counts)) {
        breakable.insert(index);
        return true;
    }
    
    
    int counts1[requestSizeAndCount.size()];
    int counts2[requestSizeAndCount.size()];
    for (int i = 1; i <= w/2; i++) {
        if (assignCountsAndDfs(w-i, h, (w-i)*h, i, h, i*h, counts, counts1, counts2, 0)) {
            breakable.insert(index);
            return true;
        }
    }
    
    if (w != h) {
        for (int i = 1; i <= h/2; i++) {
            if (assignCountsAndDfs(w, h-i, w*(h-i), w, i, w*i, counts, counts1, counts2, 0)) {
                breakable.insert(index);
                return true;
            }
        }
    }
    return false;
}

int main(void) {
    for (int casenumber = 1; true; casenumber++) {
        int n;
        cin >> n;
        if (n == 0) {
            break;
        }
        cin >> wMax >> hMax;
        if (hMax > wMax) {
            swap(wMax, hMax);
        }
        
        int p[n];
        for (int i = 0; i < n; i++) {
            cin >> p[i];
        }
        sort(p, p+n);
        
        requestSizeAndCount.clear();
        
        int requestSize = p[n-1];
        int requestCount = 1;
        for (int i = n-2; i >= 0; i--) {
            if (p[i] == requestSize) {
                requestCount++;
            } else {
                requestSizeAndCount.push_back(make_pair(requestSize, requestCount));
                requestSize = p[i];
                requestCount = 1;
            }
        }
        requestSizeAndCount.push_back(make_pair(requestSize, requestCount));
        
        int counts[requestSizeAndCount.size()];
        for (int i = 0; i < requestSizeAndCount.size(); i++) {
            counts[i] = requestSizeAndCount[i].second;
        }
        
        visit.clear();
        breakable.clear();
        cout << "Case " << casenumber << ": ";
        if (dfs(wMax, hMax, counts)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
        
    }
}
