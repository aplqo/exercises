#include <iostream>
#include <string>
using namespace std;

int s;
int main()
{
  string str;
  cin >> str;
  for (unsigned int i = 0; i < str.size(); i++) {
    switch (str[i]) {
      case '(':
        s++;
        break;
      case ')':
        if (s == 0) {
          cout << "NO";
          return 0;
        }
        s--;
        break;
      case '@':
        if (s)
          cout << "NO";
        else
          cout << "YES";
        break;
    }
  }
  return 0;
}