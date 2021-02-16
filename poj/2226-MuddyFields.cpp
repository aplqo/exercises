#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const int maxr = 50, maxe = maxr * maxr, maxv = maxr * maxr;

struct edge {
  edge() : pre(NULL) {}
  unsigned int to;
  edge* pre;
} ed[maxe * 2 + 10];
edge* head[maxv + 10];
struct {
  unsigned int cov;
  bool typ;
} mat[maxr + 10][maxr + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
namespace bipartite {
bool vis[maxv + 10];
unsigned int mat[maxv + 10];
bool find(const unsigned int x)
{
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    vis[to] = true;
    if (!mat[to] || find(mat[to])) {
      mat[to] = x;
      return true;
    }
  }
  return false;
}
inline unsigned int Hungrian(const unsigned int n, const unsigned int nl)
{
  unsigned int ret = 0;
  for (unsigned int i = 1; i <= nl; ++i) {
    if (find(i)) ++ret;
    fill(vis, vis + n, false);
  }
  return ret;
}
}  // namespace bipartite
inline unsigned int CreateGraph(const unsigned int r, const unsigned int c)
{
  unsigned int ret = 0, cnt = 1;
  for (unsigned int i = 0; i < r; ++i)
    for (unsigned int j = 0; j < c; ++j)
      if (mat[i][j].typ) {
        for (; mat[i][j].typ && j < c; ++j)
          mat[i][j].cov = cnt;
        ++cnt;
        --j;
      }
  ret = cnt - 1;
  for (unsigned int i = 0; i < c; ++i)
    for (unsigned int j = 0; j < r; ++j)
      if (mat[j][i].typ) {
        for (; mat[j][i].typ && j < r; ++j) {
          AddEdge(mat[j][i].cov, cnt);
          AddEdge(cnt, mat[j][i].cov);
        }
        ++cnt;
        --j;
      }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int r, c;
  cin >> r >> c;
  for (unsigned int i = 0; i < r; ++i) {
    cin.ignore(2, '\n');
    for (unsigned int j = 0; j < c; ++j)
      mat[i][j].typ = cin.get() == '*';
  }
  cout << bipartite::Hungrian(r * c, CreateGraph(r, c)) << endl;
  return 0;
}