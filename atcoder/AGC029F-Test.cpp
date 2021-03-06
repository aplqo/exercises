#include <numeric>
#include <unordered_set>

#include "testlib.h"
const int maxn = 2e5;

std::unordered_set<int> e[maxn + 10];
namespace Disjoint {
int fa[maxn + 10], size[maxn + 10];

static void init(const int n)
{
  std::iota(fa, fa + n + 1, 0);
  std::fill(size, size + n + 1, 1);
}
int find(const int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
bool merge(int x, int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return false;
  if (size[x] < size[y]) std::swap(x, y);
  fa[y] = x;
  size[x] += size[y];
  return true;
}
}  // namespace Disjoint

void readSet(const int n)
{
  for (int i = 0; i < n - 1; ++i) {
    const int c = inf.readInt();
    for (int j = 0; j < c; ++j)
      e[i].insert(inf.readInt());
  }
}
inline void checkEdge(const int id, const int u, const int v)
{
  static const auto check = [](const int id, const int v) {
    ouf.ensuref(e[id].count(v), "Vertex %d not in set %d", v, id);
  };
  check(id, u);
  check(id, v);
  ouf.ensuref(Disjoint::merge(u, v), "Form a cycle after adding edge (%d, %d)",
              u, v);
}
void checkAnswer(const int n)
{
  const int p1 = ouf.readInt();
  {
    const int p = ans.readInt();
    if (p == -1 && p1 != -1)
      quit(_wa, "There is no solution, but reported a solution.");
    if (p != -1 && p1 == -1)
      quit(_wa, "There is a solution, but program not found.");
    if (p == -1 && p1 == -1) quit(_ok, "No solution");
  }
  Disjoint::init(n);
  checkEdge(0, p1, ouf.readInt());
  for (int i = 1; i < n - 1; ++i) {
    const int p = ouf.readInt();
    checkEdge(i, p, ouf.readInt());
  }
}

int main(int argc, char *argv[])
{
  registerTestlibCmd(argc, argv);
  const int n = inf.readInt();
  readSet(n);
  checkAnswer(n);
  quit(_ok, "Accepted");
  return 0;
}