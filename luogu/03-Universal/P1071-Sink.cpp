#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

char pa[26], ci[26];

void err()
{
  cout << "Failed" << endl;
  exit(0);
}
int main()
{
  string p, c, t;
  cin >> c >> p >> t;
  for (unsigned int i = 0; i < p.size(); i++) {
    unsigned int t1 = c[i] - 'A', t2 = p[i] - 'A';
    if ((pa[t1] || ci[t2]) && pa[t1] != p[i]) err();
    pa[t1] = p[i];
    ci[t2] = c[i];
  }
  if (find(pa, pa + 26, 0) != pa + 26) err();
  for (unsigned int i = 0; i < t.size(); i++)
    cout << pa[t[i] - 'A'];
  return 0;
}
