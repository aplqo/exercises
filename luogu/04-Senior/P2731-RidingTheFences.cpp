#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 500, maxm = 1024;

unsigned int adj[maxn + 10][maxn + 10];
unsigned int degree[maxn + 10];
unsigned int seq[maxm * 2 + 10], *pos = seq;

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  ++adj[from][to];
  ++adj[to][from];
  ++degree[from];
  ++degree[to];
}
void dfs(const unsigned int x)
{
  for (unsigned int i = 1; i <= maxn; ++i) {
    if (!adj[i][x]) continue;
    --adj[i][x];
    --adj[x][i];
    dfs(i);
  }
  *(pos++) = x;
}
int main()
{
  unsigned int m;
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    AddEdge(u, v);
  }
  unsigned int beg;
  {
    const auto isOdd = [](unsigned int i) -> bool { return i % 2; };
    const unsigned int cnt = count_if(degree + 1, degree + maxn + 1, isOdd);
    if (cnt == 2)
      beg = find_if(degree + 1, degree + maxn + 1, isOdd) - degree;
    else
      beg = find_if(degree + 1, degree + 1 + maxn,
                    [](unsigned int i) -> bool { return i; }) -
            degree;
  }
  dfs(beg);
  reverse(seq, pos);
  for (unsigned int* i = seq; i < pos; ++i)
    cout << *i << endl;
  return 0;
}