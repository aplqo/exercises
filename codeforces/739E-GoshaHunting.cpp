#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
const int maxn = 2000;
typedef double Real;
const Real eps = 1e-8;

template <class T, int cnt>
struct List {
  T value;
  List<T, cnt - 1> next;
};
template <class T>
struct List<T, 0> {
};
template <class T, int v>
List(T, List<T, v>) -> List<T, v + 1>;

template <int c>
struct Result {
  Real value;
  List<int, c> count;
};
template <int c>
Result(Real, List<int, c>) -> Result<c>;

int n;
Real probA[maxn + 10], probB[maxn + 10];

template <int c1, int c2>
struct BinarySearch {
  static Result<c1> eval(const List<Real, c1>& costs,
                         const List<int, c2>& limits)
  {
    Real l = 0, r = 1;
    using PredReturn = Result<c1 + 1>;
    PredReturn ret;
    while (r - l >= eps) {
      const Real mid = (l + r) / 2;
      ret = BinarySearch<c1 + 1, c2 - 1>::eval(List{mid, costs}, limits.next);
      if (ret.count.value > limits.value)
        l = mid;
      else if (ret.count.value < limits.value)
        r = mid;
      else {
        l = mid;
        break;
      }
    }
    return Result{ret.value + l * limits.value, ret.count.next};
  }
};
template <int c>
struct BinarySearch<c, 0> {
  static Result<c> eval(const List<Real, c>& costs, const List<int, 0>&)
  {
    const Real costA = costs.next.value, costB = costs.value;
    Real ret = 0;
    int countA = 0, countB = 0;
    for (int i = 0; i < n; ++i) {
      const Real valA = probA[i] - costA, valB = probB[i] - costB;
      bool useA = false, useB = false;
      Real v = 0;
      if (valA >= v) {
        v = valA;
        useA = true;
        useB = false;
      }
      if (valB >= v) {
        v = probB[i] - costB;
        useA = false;
        useB = true;
      }
      if (const Real p = valA + valB - probA[i] * probB[i]; p >= v) {
        v = p;
        useA = useB = true;
      }
      ret += v;
      countA += useA;
      countB += useB;
    }
    return Result{ret, List{countB, List{countA, List<int, 0>{}}}};
  }
};

int main()
{
  std::ios::sync_with_stdio(false);
  int a, b;
  std::cin >> n >> a >> b;
  std::copy_n(std::istream_iterator<Real>(std::cin), n, probA);
  std::copy_n(std::istream_iterator<Real>(std::cin), n, probB);
  std::cout
      << std::setprecision(6) << std::fixed
      << BinarySearch<0, 2>::eval({}, List{a, List{b, List<int, 0>{}}}).value
      << "\n";
  return 0;
}