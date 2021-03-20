#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
const int maxN = 5e5, maxQ = 5e5, maxLog = 20;

class LinearBasis {
 public:
  void init() { std::memset(basis, 0, sizeof(basis)); }
  void insert(int index, unsigned int val)
  {
    for (int i = maxLog; i >= 0; --i) {
      if (!(val & (1u << i))) continue;
      if (!basis[i].val) {
        basis[i].val = val;
        basis[i].index = index;
      }
      else if (basis[i].index > index) {
        std::swap(index, basis[i].index);
        std::swap(val, basis[i].val);
      }
      val ^= basis[i].val;
    }
  }
  unsigned int maxXor(const int index)
  {
    unsigned int ret = 0;
    for (int i = maxLog; i >= 0; --i)
      if (basis[i].index <= index) ret = std::max(ret, ret ^ basis[i].val);
    return ret;
  }

 private:
  struct {
    unsigned int val;
    int index;
  } basis[maxLog + 1];
};

struct Query {
  int r;
  unsigned int answer;
} qry[maxQ + 10];
std::vector<Query *> start[maxN + 10];
unsigned int a[maxN + 10];

void answerQuery(const int n)
{
  LinearBasis lb;
  lb.init();
  for (int i = n; i; --i) {
    lb.insert(i, a[i]);
    for (Query *j : start[i])
      j->answer = lb.maxXor(j->r);
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, q;
  std::cin >> n;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, a + 1);
  std::cin >> q;
  for (Query *i = qry; i < qry + q; ++i) {
    int l;
    std::cin >> l >> i->r;
    start[l].push_back(i);
  }
  answerQuery(n);
  for (const Query *i = qry; i < qry + q; ++i)
    std::cout << i->answer << "\n";
  return 0;
}