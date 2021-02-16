#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const unsigned int maxl = 9;

unsigned int ans[10];

void add(num_t v, unsigned int res[])
{
  do {
    ++res[v % 10];
    v /= 10;
  } while (v);
}
void solve(const num_t l, const num_t r)
{
  memset(ans, 0, sizeof(ans));
  for (num_t i = l; i <= r; ++i)
    add(i, ans);
}

int main(int argc, char* argv[])
{
  ifstream fin;
  ofstream fout;
  if (argc > 1) {
    fin.open(argv[1]);
    fout.open(argv[1]);
  }
  istream& in = argc > 1 ? fin : cin;
  ostream& out = argc > 1 ? fout : cout;
  while (true) {
    num_t a, b;
    in >> a >> b;
    if (!a && !b) break;
    if (a > b) swap(a, b);
    solve(a, b);
    for (unsigned int i = 0; i < 10; ++i)
      out << ans[i] << " ";
    out << endl;
  }
  return 0;
}