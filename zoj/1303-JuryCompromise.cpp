#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
constexpr int inf = INT_MAX, ninf = INT_MIN / 2;
const int maxn = 200, maxm = 20, maxv = 20;

int f[maxn + 10][maxv * maxm * 2 + 10], ma, mi;
bool sel[maxn + 10][maxm + 10][maxv * maxm * 2 + 10];
int p[maxn + 10], d[maxn + 10];
bool ans[maxn + 10];
#define ind(a, x, y) *((a)[x] + mi + 1 + (y))

void init(const unsigned int n, const unsigned int m)
{
    memset(f, 0, sizeof(f));
    memset(sel, 0, sizeof(sel));
    memset(ans, 0, sizeof(ans));
    ma = mi = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (p[i] > d[i])
            ma += p[i] - d[i];
        else
            mi += d[i] - p[i];
    }
    for (unsigned int i = 0; i <= n; ++i)
        fill(f[i], f[i] + (ma + mi) + 2, ninf);
    ind(f, 0, 0) = 0;
}
void dp(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        const int di = p[i] - d[i];
        for (unsigned int j = min(i, m); j > 0; --j)
        {
            for (int k = -mi; k <= ma; ++k)
            {
                if (k - di < -mi || k - di > ma)
                    continue;
                const int v = ind(f, j - 1, k - di) + p[i] + d[i];
                if (v >= ind(f, j, k))
                {
                    ind(f, j, k) = v;
                    ind(sel[i], j, k) = true;
                }
            }
        }
    }
}
[[nodiscard]] int GetDiff(const unsigned int n, const unsigned int m)
{
    int ans = 0, po = inf;
    for (int i = -mi; i <= ma; ++i)
    {
        if (ind(f, m, i) < 0)
            continue;
        if (abs(i) < abs(po) || (abs(i) == abs(po) && ans < ind(f, m, i)))
        {
            ans = ind(f, m, i);
            po = i;
        }
    }
    return po;
}
void GetAnswer(const unsigned int i, const unsigned int j, const int k)
{
    if (!i || !j)
        return;
    ans[i] = ind(sel[i], j, k);
    if (ans[i])
        GetAnswer(i - 1, j - 1, k - (p[i] - d[i]));
    else
        GetAnswer(i - 1, j, k);
}
void solve(const unsigned int id)
{
    unsigned int n, m;
    cin >> n >> m;
    if (!n && !m)
        exit(0);
    for (int i = 1; i <= n; ++i)
        cin >> p[i] >> d[i];
    init(n, m);
    dp(n, m);
    const int dif = GetDiff(n, m);
    cout << "Jury #" << id << endl;
    cout << "Best jury has value " << (dif + ind(f, m, dif)) / 2 << " for prosecution and value " << (ind(f, m, dif) - dif) / 2 << " for defence:" << endl;
    GetAnswer(n, m, dif);
    for (unsigned int i = 1; i <= n; ++i)
        if (ans[i])
            cout << " " << i;
    cout << endl
         << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    for (unsigned int i = 1; i; ++i)
        solve(i);
    return 0;
}