#include <algorithm>
#include <fstream>
using namespace std;
const unsigned int maxn = 300, maxm = 1e5, maxc = 1e5, maxv = 300;

struct Store {
  unsigned int t, v, c;
} st[maxn + 1];
struct Query {
  unsigned int t, m;
  unsigned int ans;
} qry[maxm + 1];
unsigned int f[maxn + 10][maxc + 10];

void dp(const unsigned int n, const unsigned int mm)
{
  for (unsigned int i = 1; i <= n; ++i) {
    const Store cs = st[i];
    copy(f[i - 1], f[i - 1] + mm + 1, f[i]);
    for (unsigned int j = cs.c; j <= mm; ++j)
      f[i][j] = max(f[i][j], f[i - 1][j - cs.c] + cs.v);
    for (unsigned int j = 1; j <= mm; ++j)
      f[i][j] = max(f[i][j], f[i][j - 1]);
  }
}
void answerQuery(const unsigned int n, const unsigned int m)
{
  static Query* seq[maxm + 1];
  transform(qry, qry + m, seq, [](Query& i) { return &i; });
  sort(seq, seq + m, [](Query* a, Query* b) { return a->t < b->t; });
  unsigned int pos = 0;
  for (unsigned int i = 0; i < m; ++i) {
    while (pos + 1 <= n && st[pos + 1].t <= seq[i]->t)
      ++pos;
    seq[i]->ans = f[pos][seq[i]->m];
  }
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  unsigned int n, m, mm = 0;
  in >> n >> m;
  for (Store* i = st + 1; i < st + n + 1; ++i)
    in >> i->c >> i->v >> i->t;
  for (Query* i = qry; i < qry + m; ++i) {
    in >> i->t >> i->m;
    mm = max(mm, i->m);
  }
  sort(st + 1, st + n + 1,
       [](const Store& a, const Store& b) { return a.t < b.t; });
  dp(n, mm);
  answerQuery(n, m);
  for (Query* i = qry; i < qry + m; ++i)
    fans << i->ans << "\n";
  return 0;
}