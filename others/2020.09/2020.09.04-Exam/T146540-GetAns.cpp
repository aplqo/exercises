#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
using namespace std;
const unsigned int maxl = 20;

pair<bool, char> test(const string::const_iterator l,
                      const string::const_iterator r)
{
  if (r - l == 1) return make_pair(true, *l);
  const string::const_iterator mid = l + (r - l) / 2;
  const auto [okl, sucl] = test(l, mid);
  const auto [okr, sucr] = test(mid, r);
  if (!okl || !okr || sucl == sucr) return make_pair(false, 0);
  switch (sucl) {
    case 'R':
      return make_pair(true, sucr == 'S' ? sucl : sucr);
    case 'P':
      return make_pair(true, sucr == 'R' ? sucl : sucr);
    case 'S':
      return make_pair(true, sucr == 'P' ? sucl : sucr);
  }
}

string solve(const unsigned int r, const unsigned int p, const unsigned int s)
{
  string str, ans;
  str.resize(r + p + s);
  ans.resize(r + p + s);
  fill(str.begin(), str.begin() + p, 'P');
  fill(str.begin() + p, str.begin() + r + p, 'R');
  fill(str.begin() + r + p, str.end(), 'S');
  fill(ans.begin(), ans.end(), 127);
  do {
    if (test(str.cbegin(), str.cend()).first) ans = min(ans, str);
  } while (next_permutation(str.begin(), str.end()));
  return ans[0] == 127 ? "IMPOSSIBLE" : ans;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream ans(argv[2]);
  unsigned int r, p, s;
  in >> r >> p >> s;
  ans << solve(r, p, s) << "\n";
  return 0;
}