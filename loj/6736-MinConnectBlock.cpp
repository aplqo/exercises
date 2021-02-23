#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

#include "C.hpp"

std::vector<std::pair<int, int>> edges;
std::vector<int> unknown;
std::vector<int> subtree;  // vertices whose subtree is solved

//! @brief Find @c x 's descendant with minimum id
std::pair<bool, int> findDescendant(const std::vector<int>& lst, const int x)
{
  static const auto getQueryList = [](const std::vector<int>& lst,
                                      const int cnt, const int x) {
    std::vector<int> ret;
    ret.reserve(cnt + 1);
    ret.push_back(1);
    std::copy_if(lst.cbegin(), lst.cbegin() + cnt, std::back_inserter(ret),
                 [x](int p) { return p != x; });
    return ret;
  };
  if (x == 1)
    return lst.empty() ? std::make_pair(false, 0)
                       : std::make_pair(true, lst.back());
  if (!ask(getQueryList(lst, lst.size(), x), x))
    return std::make_pair(false, 0);

  int l = 1, r = lst.size();
  while (l <= r) {
    const int mid = (l + r) >> 1;
    if (ask(getQueryList(lst, mid, x), x))
      r = mid - 1;
    else
      l = mid + 1;
  }
  return std::make_pair(true, lst[r]);
}
void solveSubtree(const int x)
{
  static const auto erase = [](std::vector<int>& v, const int val) {
    v.erase(std::find(v.cbegin(), v.cend(), val));
  };
  while (true) {
    const auto [suc, dec] = findDescendant(unknown, x);
    if (!suc)
      break;
    else
      solveSubtree(dec);
  }

  while (true) {
    const auto [suc, dec] = findDescendant(subtree, x);
    if (!suc)
      break;
    else {
      edges.emplace_back(x, dec);
      erase(subtree, dec);
    }
  }
  if (x > 1) {
    subtree.emplace_back(x);
    erase(unknown, x);
  }
}

std::vector<std::pair<int, int>> work(const int n)
{
  unknown.resize(n - 1);
  std::iota(unknown.begin(), unknown.end(), 2);
  solveSubtree(1);
  return edges;
}