#include <cmath>
#include <vector>

#include "testlib.h"

const int maxn = 1e5;

std::vector<long long> val;

class Tree {
 public:
  struct Edge {
    int to;
    const Edge *pre;
  };
  const Edge *head[maxn + 10];

  int read(const int n)
  {
    int root;
    for (int i = 1; i <= n; ++i) {
      const int p = inf.readInt();
      if (p == -1)
        root = i;
      else
        addEdge(p, i);
    }
    return root;
  }
  template <int first>
  long long dfs(const int x)
  {
    long long sum = val[x - 1];
    for (const Edge *i = head[x]; i; i = i->pre)
      sum += dfs<first>(i->to);
    ouf.ensuref(fabs(sum) == 1, "subtree sum of vertex %d in tree %c is %lld",
                x, first ? 'A' : 'B', sum);
    return sum;
  }

 private:
  void addEdge(const int from, const int to)
  {
    new (cur) Edge{to, head[from]};
    head[from] = cur++;
  }
  Edge ed[maxn + 10], *cur = ed;
} tree[2];

void checkVerdict()
{
  const std::string exp = ans.readWord(),
                    out = ouf.readToken("IMPOSSIBLE|POSSIBLE", "Verdict");
  ouf.ensuref(exp == out, "Wrong verdict. Expected=%s Read=%s", exp.c_str(),
              out.c_str());
  if (exp == "IMPOSSIBLE") quit(_ok, "No solution");
}

int main(int argc, char *argv[])
{
  registerTestlibCmd(argc, argv);
  checkVerdict();
  const int n = inf.readInt();
  const int rootA = tree[0].read(n);
  const int rootB = tree[1].read(n);
  val = ouf.readLongs(n);
  tree[0].dfs<true>(rootA);
  tree[1].dfs<false>(rootB);
  quit(_ok, "Accpeted");
}