#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 200000;
typedef long double real;

real x[maxn + 10], r[maxn + 10];

void solve(const unsigned int n)
{
    static unsigned int stk[maxn + 10];
    static const auto sq = [](real v) { return v * v; };
    static const auto maxR = [](unsigned int lst, unsigned int cur) {
        return sq(x[cur] - x[lst]) / (4 * r[lst]);
    };
    unsigned int* top = stk;
    for (unsigned int i = 0; i < n; ++i)
    {
        real mv = r[i];
        while (top > stk)
        {
            mv = min(mv, maxR(*(top - 1), i));
            if (mv > r[*(top - 1)])
                --top;
            else
                break;
        }
        *(top++) = i;
        r[i] = mv;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
        cin >> x[i] >> r[i];
    solve(n);
    cout.setf(ios::fixed);
    cout.precision(3);
    copy(r, r + n, ostream_iterator<real>(cout, "\n"));
    return 0;
}