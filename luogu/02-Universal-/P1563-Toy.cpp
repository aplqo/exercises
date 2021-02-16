#include <iostream>
#include <string>
using namespace std;

struct peo {
  string name;
  unsigned int f;
} p[100000];

int main()
{
  int n, m;
  cin >> n >> m;
  for (peo* i = p; i < p + n; i++)
    cin >> i->f >> i->name;
  peo* pos = p;
  for (int i = 0; i < m; i++) {
    int num;
    unsigned int d;
    cin >> d >> num;
    if (d ^ pos->f) {
      pos += num;
      if (pos > p + n || pos == p + n) pos -= n;
    }
    else {
      pos -= num;
      if (pos < p) pos += n;
    }
  }
  cout << pos->name;
  return 0;
}
