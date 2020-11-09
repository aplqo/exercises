#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 200000;

unsigned int val[maxn + 10], inc[maxn + 10], l[maxn + 10];

class FTree
{
#define lowbit(x) ((x) & -(x))
public:
    void update(unsigned int pos, const unsigned int n, const unsigned int v)
    {
        for (; pos <= n; pos += lowbit(pos))
            val[pos] = max(val[pos], v);
    }
    unsigned int prefix(unsigned int pos)
    {
        unsigned int ret = 0;
        for (; pos; pos -= lowbit(pos))
            ret = max(ret, val[pos]);
        return ret;
    }

private:
    unsigned int val[maxn * 2 + 10] = {};
#undef lowbit
};

static void
discretize(const unsigned int n)
{
    static unsigned int tmp[maxn * 2 + 10];
    copy(val, val + n, tmp);
    copy(inc, inc + n, tmp + n);
    sort(tmp, tmp + n * 2);
    unsigned int* cur = unique(tmp, tmp + n * 2);
    for (unsigned int* i = val; i < val + n; ++i)
        *i = lower_bound(tmp, cur, *i) - tmp + 1;
    for (unsigned int* i = inc; i < inc + n; ++i)
        *i = lower_bound(tmp, cur, *i) - tmp + 1;
}
static void init(const unsigned int n, const unsigned int x)
{
    transform(val, val + n, inc, [x](const unsigned int i) { return i + x; });
    discretize(n);
}
void lds(const unsigned int n)
{
    static FTree ft;
    for (int i = n - 1; i >= 0; --i)
    {
        const unsigned int rv = 2 * n + 1 - val[i];
        l[i] = ft.prefix(rv - 1) + 1;
        ft.update(rv, n * 2, l[i]);
    }
}
unsigned int solve(const unsigned int n)
{
    static FTree ft;
    unsigned int ret = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        ret = max(ret, ft.prefix(inc[i] - 1) + l[i]);
        ft.update(val[i], n * 2, ft.prefix(val[i] - 1) + 1);
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, x;
    cin >> n >> x;
    copy_n(istream_iterator<unsigned int>(cin), n, val);
    init(n, x);
    lds(n);
    cout << solve(n) << "\n";
    return 0;
}