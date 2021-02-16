#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>

#include "debug_tools/tester.h"
using namespace std;
using namespace apdebug::tester;
const unsigned int maxn = 5e4;

unsigned int typ[maxn + 10];

template <class... Args>
void fail(const unsigned int id, Args... args)
{
  print(cout, "Test ", id, ": ", forward<Args>(args)...);
  exit(1);
}

unsigned int readInput(ifstream& in)
{
  unsigned int n, q;
  in >> n >> q;
  copy_n(istream_iterator<unsigned int>(in), n, typ);
  return n;
}
bool checkVerdict(const unsigned int id, ifstream& out, ifstream& fans)
{
  char c, ex;
  out >> c;
  fans >> ex;
  if (c != ex) fail(id, "Wrong verdict. Expected=", ex, " Read=", c);
  return ex == 'Y';
}
void checkPermutation(const unsigned int id, const unsigned int n,
                      ifstream& out)
{
  static unsigned int vis[maxn + 10];
  unsigned int lst;
  out >> lst;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int cur;
    out >> cur;
    if (vis[cur] == id + 1)
      fail(id, "Wring solution. ", cur, " selected twice.");
    if (typ[cur] == typ[lst])
      fail(id, "Wrong solution. p[", i - 1, "]=", lst, " p[", i, "]=", cur,
           " but type[", lst, "]=type[", cur, "]=", typ[cur]);
    lst = cur;
    vis[cur] = id + 1;
  }
}

int main(int, char* argv[])
{
  ifstream in(argv[1]), out(argv[2]), ans(argv[3]);
  unsigned int t;
  in >> t;
  for (unsigned int i = 0; i < t; ++i) {
    const unsigned int n = readInput(in);
    if (checkVerdict(i, out, ans)) checkPermutation(i, n, out);
  }
  return 0;
}