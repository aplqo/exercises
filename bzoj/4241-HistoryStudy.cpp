#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
using namespace std;
using num_t = unsigned long long;
const int maxn = 1e5, maxb = 317;

unsigned int x[maxn + 10];
unsigned int cnt[maxn + 10][maxb + 10], block;
num_t f[maxb + 10][maxn + 10];
unsigned int orig[maxn + 10];
unsigned int app[maxb * 2 + 10], tmp[maxn + 10], vis[maxn + 10], *cur = app;

inline void discretize(const unsigned int n)
{
    copy(x, x + n, orig);
    sort(orig, orig + n);
    unsigned int* e = unique(orig, orig + n);
    for (unsigned int* i = x; i < x + n; ++i)
        *i = lower_bound(orig, e, *i) - orig;
}
inline void init(const unsigned int n)
{
    block = sqrt(n);
    f[0][0] = orig[x[0]];
    cnt[x[0]][0] = 1;
    for (unsigned int i = 1; i < n; ++i)
    {
        ++cnt[x[i]][i / block];
        unsigned int t = 0;
        if (i % block)
            f[i / block][i] = orig[x[i]];
        for (int j = i / block; j >= 0; --j)
        {
            t += cnt[x[i]][j];
            f[j][i] = max(f[j][i - 1], num_t(t) * orig[x[i]]);
        }
    }
    for (unsigned int i = 0; i < n; ++i)
        partial_sum(cnt[i], cnt[i] + (n - 1) / block + 1, cnt[i]);
}
inline num_t query(const unsigned int a, const unsigned int b)
{
    static unsigned int stamp = 0;
    const unsigned int inda = (a + block) / block, indb = b / block;
    num_t ret = f[inda][b];
    static const auto check = [](const unsigned int a, const unsigned int b) {
        for (unsigned int* i = x + a; i < x + b; ++i)
        {
            if (vis[*i] == stamp)
                ++tmp[*i];
            else
            {
                tmp[*i] = 1;
                vis[*i] = stamp;
                *cur = *i;
                ++cur;
            }
        }
    };

    ++stamp;
    if (inda < indb)
    {
        check(a, inda * block);
        check(indb * block, b + 1);
    }
    else
        check(a, b + 1);
    for (unsigned int* i = app; i < cur; ++i)
    {
        if (inda < indb)
            tmp[*i] += cnt[*i][indb - 1] - cnt[*i][inda - 1];
        ret = max(ret, num_t(tmp[*i]) * orig[*i]);
    }
    cur = app;
    return ret;
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, q;
    cin >> n >> q;

    for_each(x, x + n, [](unsigned int& i) -> void { cin >> i; });
    discretize(n);
    init(n);

    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        cout << query(a - 1, b - 1) << endl;
    }
    return 0;
}