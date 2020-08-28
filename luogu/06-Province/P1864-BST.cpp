#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 70;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Node
{
    unsigned int val, tim, w;
} nod[maxn + 1];
unsigned int a[maxn + 10];
unsigned long long tim[maxn + 10];

static void discretize(const unsigned int n)
{
    static unsigned int tmp[maxn + 1];
    copy(a + 1, a + n + 1, tmp);
    sort(tmp, tmp + n);
    unsigned int* const e = unique(tmp, tmp + n);
    for (unsigned int* i = a + 1; i < a + n + 1; ++i)
        *i = lower_bound(tmp, e, *i) - tmp;
}
unsigned long long dp(const unsigned int n, const unsigned int cost)
{
    static unsigned long long f[maxn + 10][maxn + 10][maxn + 10];
    static const auto sumT = [](unsigned int l, unsigned int r) { return tim[r] - tim[l - 1]; };
    for (unsigned int i = 1; i <= n; ++i)
    {
        fill(f[i][i], f[i][i] + a[i] + 1, sumT(i, i));
        fill(f[i][i] + a[i] + 1, f[i][i] + n, sumT(i, i) + cost);
    }
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int m = 0; m < n; ++m)
            for (unsigned int l = 1; l + i - 1 <= n; ++l)
            {
                const unsigned int r = l + i - 1;
                f[l][r][m] = inf;
                for (unsigned int k = l; k <= r; ++k)
                {
                    if (a[k] >= m)
                        f[l][r][m] = min(f[l][r][m], f[l][k - 1][a[k]] + f[k + 1][r][a[k]]);
                    f[l][r][m] = min(f[l][r][m], f[l][k - 1][m] + f[k + 1][r][m] + cost);
                }
                f[l][r][m] += sumT(l, r);
            }
    return *min_element(f[1][n], f[1][n] + n);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (Node* i = nod + 1; i < nod + 1 + n; ++i)
        cin >> i->val;
    for (Node* i = nod + 1; i < nod + 1 + n; ++i)
        cin >> i->w;
    for (Node* i = nod + 1; i < nod + 1 + n; ++i)
        cin >> i->tim;
    sort(nod + 1, nod + 1 + n, [](const Node& a, const Node& b) { return a.val < b.val; });
    for (unsigned int i = 1; i <= n; ++i)
    {
        a[i] = nod[i].w;
        tim[i] = nod[i].tim;
    }
    partial_sum(tim + 1, tim + 1 + n, tim + 1);
    discretize(n);
    cout << dp(n, k) << "\n";
    return 0;
}