#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <numeric>
using namespace std;
const unsigned int maxn = 5e5;
typedef double real;
constexpr real eps = 1e-8;

real a[maxn + 1];
unsigned int cnt[maxn + 1];

static void init(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        const real v = floor(a[i]);
        if (fabs(a[i] - v) <= eps)
        {
            cnt[i] = 1;
            a[i] = 0;
        }
        else
            a[i] = v - a[i];
    }
    partial_sum(a, a + n + 1, a);
    partial_sum(cnt, cnt + 1 + n, cnt);
}
real query(const unsigned int l, const unsigned int r)
{
    real lsum = a[r] - a[l - 1];
    const unsigned int req = fabs(lsum), hlen = (r + 1 - l) / 2, cntz = (cnt[r] - cnt[l - 1]);
    if (req == hlen)
        return fabs(lsum + hlen);
    if (req < hlen)
    {
        lsum += req;
        if (hlen - req > cntz)
            lsum += hlen - req - cntz;
        return min(fabs(lsum), fabs(lsum + 1));
    }
    return fabs(lsum + hlen);
}
int main(int, char* argv[])
{
    unsigned int m, q;
    scanf("%u%u", &m, &q);
    for (real* i = a + 1; i < a + 1 + m; ++i)
        scanf("%lf", i);
    init(m);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int l, r;
        scanf("%u%u", &l, &r);
        printf("%.3lf\n", query(l, r));
    }
    return 0;
}