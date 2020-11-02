#ifdef EnHeader
#include "gap.h"
#endif
#include <climits>
#include <fstream>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long inf = 1e18;

#ifndef EnHeader
void MinMax(long long s, long long t, long long* mn, long long* mx)
{
    cout << "? " << s << " " << t << endl;
    cin >> *mn >> *mx;
}
#endif

inline pair<long long, long long> MinMax(const unsigned long long s, const unsigned long long t)
{
    long long mn, mx;
    MinMax(s, t, &mn, &mx);
    return make_pair(mn, mx);
}

namespace Forall
{
    long long a[maxn + 10];

    long long findGap(const unsigned int n)
    {
        {
            const auto [mn, mx] = MinMax(0, inf);
            a[1] = mn;
            a[n] = mx;
        }
        for (unsigned int l = 2, r = n - 1; l <= r; ++l, --r)
        {
            const auto [mn, mx] = MinMax(a[l - 1] + 1, a[r + 1] - 1);
            a[l] = mn;
            a[r] = mx;
        }
        long long ret = 0;
        for (unsigned int i = 2; i <= n; ++i)
            ret = max(ret, a[i] - a[i - 1]);
        return ret;
    }
}
namespace Step
{
    long long findGap(const unsigned int n)
    {
        const auto [mn, mx] = MinMax(0, inf);
        const long long step = (mx - mn) / (n - 1);
        long long ans = (mx - mn) / (n - 1), lst = mn;
        for (long long pos = mn + 1; pos < mx; pos += step + 1)
        {
            const auto [miv, mxv] = MinMax(pos, pos + step);
            if (miv != -1)
            {
                ans = max(ans, miv - lst);
                lst = mxv;
            }
        }
        return ans;
    }
}
long long findGap(const int t, const int n)
{
    if (t == 1)
        return Forall::findGap(n);
    else
        return Step::findGap(n);
}
#ifndef Enheader

int main(int, char* argv[])
{
    ios_base::sync_with_stdio(false);
#ifdef APTEST
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    cin.rdbuf(fin.rdbuf());
    cout.rdbuf(fout.rdbuf());
#endif
    unsigned int t, n;
    cin >> t >> n;
    const long long res = findGap(t, n);
    cout << "! " << res << endl;
    return 0;
}

#endif