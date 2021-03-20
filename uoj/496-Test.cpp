#include <algorithm>
#include <numeric>
#include <unordered_set>

#include "testlib.h"
const int maxn = 500000;

std::unordered_set<int> to[maxn + 10];
int degree[maxn + 10];

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

void readInput(const int n)
{
  static const auto addEdge = [](int u, int v) {
    to[u].insert(v);
    to[v].insert(u);
  };
  for (int i = n - 3; i; --i) {
    const int u = inf.readInt();
    addEdge(u, inf.readInt());
  }
  for (int i = 1; i < n; ++i)
    addEdge(i, i + 1);
  addEdge(n, 1);
}
void checkEdge(const int u, const int v)
{
  ouf.ensuref(to[u].contains(v), "Edge (%d, %d) not found.", u, v);
  ouf.ensuref(Disjoint::merge(u, v), "Form a cycle after adding edge (%d, %d)",
              u, v);
  ++degree[u];
  ++degree[v];
}
void checkOutput(const int n)
{
  const int f = ouf.readInt();
  if (n == 3) {
    if (f != -1) quit(_wa, "There is no solution, but program reported one.");
    quit(_ok, "No solution");
  }
  if (f == -1) quit(_wa, "There is a solution, but program not found.");
  if (f <= 0 || f > n) quitf(_wa, "Unexpected numer %d", f);
  readInput(n);
  Disjoint::init(n);
  checkEdge(f, ouf.readInt(1, n));
  for (int i = n - 2; i; --i) {
    const int p = ouf.readInt(1, n);
    checkEdge(p, ouf.readInt(1, n));
  }
}
void checkDegree(const int n)
{
  for (int i = 1; i <= n; ++i)
    ouf.ensuref(degree[i] != 2, "Degree of vertex %d is %d", i, degree[i]);
}

int main(int argc, char *argv[])
{
  dup2(STDOUT_FILENO, STDERR_FILENO);
  registerTestlibCmd(argc, argv);
  const int n = inf.readInt();
  checkOutput(n);
  checkDegree(n);
  quit(_ok, "Accepted");
  return 0;
}