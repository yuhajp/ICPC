#include "a.h"

int main(void) {
  char line[120];
  Environment environment;
  
  for (int casenumber = 1; true; casenumber++) {
    string line;
    getline(cin, line);
    if (line[0] == '#') {
      break;
    }
    cout << "Case " << casenumber << ": " << line << endl;;
    Element result = solve(environment, line);
    print(result);
  }
}
