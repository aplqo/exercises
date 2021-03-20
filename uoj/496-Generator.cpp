#include <cmath>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 500000;
typedef __gnu_pbds::tree<int, __gnu_pbds::null_type, std::less<int>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>
    Tree;

std::ofstream in;

void generate(Tree& t)
{
  if (t.size() <= 3) return;
  int u = GetRandom<size_t>(0, t.size() - 1), v;
  do {
    v = GetRandom<size_t>(0, t.size() - 1);
  } while (std::min(abs(u - v), static_cast<int>(t.size()) - abs(u - v)) < 2);
  if (u > v) std::swap(u, v);
  u = *t.find_by_order(u);
  v = *t.find_by_order(v);
  in << u << " " << v << "\n";

  Tree t1;
  {
    Tree t2;
    t.split(v, t1);
    t.split(u - 1, t2);
    generate(t2);
  }
  t.join(t1);

  t.insert(u);
  t.insert(v);
  generate(t);
}

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(3, atoi(argv[1]));
  in.open(argv[2]);
  in << n << "\n";
  Tree t;
  for (int i = 1; i <= n; ++i)
    t.insert(i);
  generate(t);
  return 0;
}