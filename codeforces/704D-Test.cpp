#include <climits>
#include <cmath>
#include <unordered_map>
#include <utility>

#include "testlib.h"
const int maxn = 100000, maxm = 100000;

struct Constraint {
  int type, val, d;
} con[maxm + 10];
struct Point {
  int x, y;
} p[maxn + 10];
int n, m, r, b;

std::pair<long long, std::string> readOutput()
{
  const int o = ouf.readLong(-1, LLONG_MAX, "cost");
  const int a = ans.readLong();
  if (o == -1) {
    if (a != -1)
      quit(_wa, "There is a solution, but program not found");
    else
      quit(_ok, "No solution");
  }
  else if (o > a)
    quitf(_wa, "There exists a better solution. Output=%lld Answer=%lld", o, a);
  ouf.readEoln();
  return std::make_pair(o, ouf.readString("[rb]*"));
}
inline void add(std::unordered_map<int, int> &m, const int p, const int v)
{
  auto it = m.find(p);
  if (it == m.end()) it = m.insert({p, 0}).first;
  it->second += v;
}
inline int get(const std::unordered_map<int, int> &m, const int p)
{
  auto it = m.find(p);
  return it == m.end() ? 0 : it->second;
}
void readData()
{
  n = inf.readInt();
  m = inf.readInt();
  r = inf.readInt();
  b = inf.readInt();
  for (Point *i = p; i < p + n; ++i)
    *i = Point{.x = inf.readInt(), .y = inf.readInt()};
  for (Constraint *i = con; i < con + m; ++i)
    *i = Constraint{
        .type = inf.readInt(), .val = inf.readInt(), .d = inf.readInt()};
}
void checkSolution(const std::string s, const long long cost)
{
  std::unordered_map<int, int> countX, countY;
  ouf.ensuref(s.size() == n, "Wrong output size. N=%d length=%zd", n, s.size());
  long long acc = 0;
  for (size_t i = 0; i < s.size(); ++i)
    if (s[i] == 'r') {
      add(countX, p[i].x, 1);
      add(countY, p[i].y, 1);
      acc += r;
    }
    else {
      add(countX, p[i].x, -1);
      add(countY, p[i].y, -1);
      acc += b;
    }
  ouf.ensuref(acc == cost, "Wrong solution. Actual=%lld Read=%lld", cost, acc);
  for (const Constraint *i = con; i < con + m; ++i)
    if (i->type == 1) {
      const auto p = abs(get(countX, i->val));
      ouf.ensuref(p <= i->d,
                  "diffenence of line X=%d exceed limit. Limit=%d Actual=%d",
                  i->val, i->d, p);
    }
    else {
      const auto p = abs(get(countY, i->val));
      ouf.ensuref(p <= i->d,
                  "difference of line Y=%d exceed limit. Limit=%d Acutal=%d",
                  i->val, i->d, p);
    }
}

int main(int argc, char *argv[])
{
#ifndef UseStdErr
  dup2(STDOUT_FILENO, STDERR_FILENO);
#endif
  registerTestlibCmd(argc, argv);
  const auto [cst, sol] = readOutput();
  readData();
  checkSolution(sol, cst);
  quit(_ok, "Accepted");
  return 0;
}