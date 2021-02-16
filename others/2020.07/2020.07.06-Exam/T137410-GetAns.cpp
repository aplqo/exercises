#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxn = 5000;

struct object {
  unsigned int v, w;
} obj[maxn + 1];
const object* seq[maxn + 1];

int sum(const unsigned int n)
{
  int acc = 0, ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    if (seq[i]->v > acc) {
      ret += seq[i]->v - acc;
      acc += seq[i]->w;
    }
  return ret;
}
int solve(const unsigned int n)
{
  transform(obj, obj + n, seq, [](const object& o) { return &o; });
  int ret = 0;
  do {
    ret = max(ret, sum(n));
  } while (next_permutation(seq, seq + n));
  return ret;
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int n;
  in >> n;
  for (object* i = obj; i < obj + n; ++i)
    in >> i->v >> i->w;
  ans << solve(n) << endl;
  return 0;
}