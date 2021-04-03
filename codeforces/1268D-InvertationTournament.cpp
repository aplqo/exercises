#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <numeric>
#include <utility>
const int maxN = 2000;
const unsigned int mod = 998244353;
constexpr int inf = INT_MAX;

class Array {
 public:
  inline const int *begin() const { return data; }
  inline const int *end() const { return endPtr; }
  inline void push_back(const int x) { *(endPtr++) = x; }
  inline size_t size() const { return endPtr - data; }
  template <class T>
  inline void foreach (T fun) const
  {
    std::for_each<const int *>(data, endPtr, fun);
  }

 private:
  int data[maxN + 10], *endPtr = data;
};
Array edgeIn[maxN + 10], edgeOut[maxN + 10];
int originDegree[maxN + 10], degree[maxN + 10];

inline void reset(const int n)
{
  std::memcpy(degree, originDegree, sizeof(degree[0]) * n);
}
void readGraph(const int n)
{
  for (int i = 0; i < n; ++i) {
    static char str[maxN + 10];
    std::cin >> str;
    for (int j = 0; j < n; ++j)
      if (str[j] == '1') {
        edgeIn[j].push_back(i);
        edgeOut[i].push_back(j);
        ++originDegree[i];
      }
  }
}
bool isStrongConnect(const int n)
{
  std::sort(degree, degree + n);
  int sum = 0;
  for (int i = 0; i + 1 < n; ++i) {
    sum += degree[i];
    if (i * (i + 1) / 2 == sum) return false;
  }
  return true;
}

void reverseSet(const int n, const unsigned int set)
{
#define exist(i, s) (((s) >> (i)) & 0x01u)
  for (int i = 0; i < n; ++i)
    for (auto to : edgeOut[i])
      if (exist(i, set) ^ exist(to, set)) {
        --degree[i];
        ++degree[to];
      }
#undef exist
}
inline bool checkSet(const int n, const unsigned int set)
{
  reset(n);
  reverseSet(n, set);
  return isStrongConnect(n);
}

void reverseSingle(const int vertex)
{
  edgeOut[vertex].foreach ([](int p) { ++degree[p]; });
  edgeIn[vertex].foreach ([](int p) { --degree[p]; });
  degree[vertex] = edgeIn[vertex].size();
}
inline bool checkSingle(const int n, const int vertex)
{
  reset(n);
  reverseSingle(vertex);
  return isStrongConnect(n);
}

unsigned int factor(const unsigned int n)
{
  unsigned int ret = 1;
  for (unsigned int i = 2; i <= n; ++i)
    ret = (ret * i) % mod;
  return ret;
}
std::pair<int, int> enumSet(const int n)
{
  int count = inf, solution = 0;
  const unsigned int msk = 1u << n;
  for (unsigned int i = 1; i < msk; ++i) {
    const int cnt = __builtin_popcount(i);
    if (cnt <= count && checkSet(n, i)) {
      if (cnt == count)
        ++solution;
      else {
        solution = 1;
        count = cnt;
      }
    }
  }
  return std::make_pair(count,
                        count != inf ? (solution * factor(count)) % mod : 0);
}
std::pair<int, int> enumVertex(const int n)
{
  int solution = 0;
  for (int i = 0; i < n; ++i)
    solution += checkSingle(n, i);
  return std::make_pair(1, solution);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  readGraph(n);
  reset(n);
  if (isStrongConnect(n)) {
    std::cout << "0 1\n";
    return 0;
  }
  const auto [count, solution] = n <= 6 ? enumSet(n) : enumVertex(n);
  if (count == inf)
    std::cout << "-1\n";
  else
    std::cout << count << " " << solution << "\n";
  return 0;
}