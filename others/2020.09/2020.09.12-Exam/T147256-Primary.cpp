#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 25;
constexpr unsigned long long mod = 1e9 + 9;

unsigned long long fac[maxn + 10], ans;
unsigned int a[maxn + 10], x;
unsigned int n;

static void getFactor(const unsigned int n)
{
    fac[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        fac[i] = (fac[i - 1] * i) % mod;
}
template <class Operate>
void dfs(const unsigned int pos, const unsigned int acc, const unsigned int cnt)
{
    static Operate op;
    if (pos == n)
    {
        if (cnt && acc == x)
            ans = (ans + fac[cnt]) % mod;
        return;
    }
    dfs<Operate>(pos + 1, acc, cnt);
    dfs<Operate>(pos + 1, op(acc, a[pos]), cnt + 1);
}
unsigned int readVal(const unsigned int m)
{
    unsigned int ret = 0;
    for (int j = m - 1; j >= 0; --j)
    {
        unsigned int v;
        cin >> v;
        ret |= v << j;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    char op;
    unsigned int m;
    cin >> op >> n >> m;
    for (unsigned int* i = a; i < a + n; ++i)
        *i = readVal(m);
    x = readVal(m);
    getFactor(n);
    switch (op)
    {
    case '&':
        dfs<bit_and<unsigned int>>(0, UINT_MAX, 0);
        break;
    case '|':
        dfs<bit_or<unsigned int>>(0, 0, 0);
        break;
    case '^':
        dfs<bit_xor<unsigned int>>(0, 0, 0);
        break;
    }
    cout << ans << "\n";
    return 0;
}