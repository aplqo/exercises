#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 25;

string s[25];

int main()
{
  int n;
  cin >> n;
  for_each(s, s + n, [](string &i) { cin >> i; });
  sort(s, s + n, [](const string &a, const string &b) -> bool {
    return (a + b) > (b + a);
  });
  for_each(s, s + n, [](const string &i) { cout << i; });
  return 0;
}