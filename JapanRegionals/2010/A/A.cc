#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

void iter(const string& key,
          const map<string, vector<string> >& dict,
          map<string, set<string> >& valueMap)
{
    if (valueMap.count(key))
        return;

    if (!dict.count(key))
        return;

    const vector<string> values = dict.find(key)->second;
    set<string> valueSet;
    for (int i = 0; i < values.size(); ++i) {
        iter(values[i], dict, valueMap);
        if (valueMap.count(values[i]))
            valueSet.insert(valueMap[values[i]].begin(), valueMap[values[i]].end());
        else
            valueSet.insert(values[i]);
    }
    valueMap[key] = valueSet;
}

int solve(const string& key, 
          const map<string, vector<string> >& dict)
{
    map<string, set<string> > valueMap;
    iter(key, dict, valueMap);
    return valueMap[key].size();
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        map<string, vector<string> > dict;
        string origin;
        for (int i = 0; i < N; ++i) {
            string str; cin >> str;
            for (int j = 0; j < str.length(); ++j) {
                if (str[j] == ':' || str[j] == ',' || str[j] == '.')
                    str[j] = ' ';
            }
            istringstream ss(str);
            string key, value;
            vector<string> values;
            ss >> key;
            while (ss >> value) {
                values.push_back(value);
            }
            
            dict[key] = values;
            if (i == 0)
                origin = key;
        }

        cout << solve(origin, dict) << endl;
    }
    return 0;
}
