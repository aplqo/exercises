#include <iostream>
using namespace std;
const int use[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};
int n, num[2], ans;

template <int dep>
class fun {
 public:
  inline static void func(int last);
};
template <>
class fun<2> {
 public:
  inline static void func(int last);
};

template <int pos, int dep, int f, int rat>
class tr {
 public:
  inline static void tbit(int last);
};
template <int pos, int f, int rat>
class tr<pos, 0, f, rat> {
 public:
  inline static void tbit(int last);
};
template <int pos, int dep, int f, int rat>
inline void tr<pos, dep, f, rat>::tbit(int last)
{
  for (int i = (dep == f ? 1 : 0); i < 10; i++) {
    if (use[i] > last) continue;
    num[pos] += i * rat;
    tr<pos, dep - 1, f, rat * 10>::tbit(last - use[i]);
    num[pos] -= i * rat;
  }
}
template <int pos, int f, int rat>
inline void tr<pos, 0, f, rat>::tbit(int last)
{
  fun<pos + 1>::func(last);
}

template <int dep>
inline void fun<dep>::func(int last)
{
  tr<dep, 1, 0, 1>::tbit(last);
  tr<dep, 2, 1, 1>::tbit(last);
  tr<dep, 3, 1, 1>::tbit(last);
}
inline void fun<2>::func(int last)
{
  int t = num[0] + num[1];
  do {
    last -= use[t % 10];
    t /= 10;
  } while (t);
  if (last == 0) ans++;
}

int main()
{
  cin >> n;
  n -= 4;
  fun<0>::func(n);
  cout << ans;
  return 0;
}
