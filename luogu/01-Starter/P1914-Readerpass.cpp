#include <iostream>
#include <string>
using namespace std;
int main()
{
  int n;
  string s;
  cin >> n >> s;
  n %= 26;
  for (auto& i : s) {
    i += n;
    if ((unsigned char)i > 'z') i = i - 'z' + 'a' - 1;
  }
  cout << s;
  return 0;
}
