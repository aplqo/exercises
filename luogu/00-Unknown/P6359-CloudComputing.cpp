#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000, maxm = 2000, maxc = 50;
constexpr unsigned int maxo = maxn + maxm, maxw = maxn * maxc;

struct Object
{
    unsigned int f;
    long long v;
    int c;
} ob[maxn + maxm + 10];

long long dp(const int n, const int mc)
{
    static long long f[maxw + 10];
    for (int i = 0; i <= n; ++i)
        fill(f + 1, f + mc + 1, LLONG_MIN / 2);
    int sumc = 0;
    for (int i = 1; i <= n; ++i)
    {
        const Object o = ob[i];
        if (o.c > 0)
        {
            sumc += o.c;
            for (int j = sumc; j >= o.c; --j)
                f[j] = max(f[j], f[j - o.c] + o.v);
        }
        else
        {
            for (int j = 0; j - o.c <= sumc; ++j)
                f[j] = max(f[j], f[j - o.c] + o.v);
        }
    }
    return *max_element(f, f + mc + 1);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, mc = 0;
    cin >> n;
    for (Object* i = ob + 1; i < ob + n + 1; ++i)
    {
        cin >> i->c >> i->f >> i->v;
        i->v = -i->v;
        mc += i->c;
    }
    cin >> m;
    for (Object* i = ob + n + 1; i < ob + n + m + 1; ++i)
    {
        cin >> i->c >> i->f >> i->v;
        i->c = -i->c;
    }
    sort(ob + 1, ob + n + m + 1, [](const Object& a, const Object& b) { return a.f != b.f ? a.f > b.f : a.c > b.c; });
    cout << dp(n + m, mc) << "\n";
    return 0;
}