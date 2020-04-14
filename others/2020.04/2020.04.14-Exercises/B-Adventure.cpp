/*
* Luogu team T129620: adventure
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <limits>
using namespace std;
constexpr double ninf = numeric_limits<double>::min();
const int maxn = 1000;

long long grt[maxn + 10], val[maxn + 10];
double f[maxn + 10][maxn + 10], po[maxn + 10] = { 1 };

void init(const unsigned int n, const unsigned int m, const double k)
{
    for (unsigned int i = 1; i <= n; ++i)
        po[i] = po[i - 1] * k;
    for (unsigned int i = 0; i < m; ++i)
        fill(f[i] + 1, f[i] + 1 + n, ninf);
    for (unsigned int i = m; i <= n; ++i)
        fill(f[i], f[i] + 1 + n, ninf);
    {
        deque<unsigned int> q;
        const auto push = [&q](const unsigned int p) -> void {
            while (!q.empty() && val[q.back()] < val[p])
                q.pop_back();
            q.emplace_back(p);
        };
        const auto pop = [&q](const unsigned int p) -> void {
            while (!q.empty() && q.front() < p)
                q.pop_front();
        };
        for (unsigned int i = 1; i <= m; ++i)
            push(i);
        for (unsigned int i = 1; i <= n - m + 1; ++i)
        {
            pop(i);
            push(i + m - 1);
            grt[i] = val[q.front()];
        }
    }
}
double dp(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i < m; ++i)
        for (unsigned int j = 1; j <= i; ++j)
            f[i][j] = max(f[i - 1][j], f[i - 1][j - 1] + val[i] * po[j - 1]);
    for (unsigned int i = m; i <= n; ++i)
        for (unsigned int j = ceil(1.0 * i / m); j <= i; ++j)
        {
            if (grt[i - m + 1] > 0)
                f[i][j] = max(f[i - 1][j], f[i - 1][j - 1] + val[i] * po[j - 1]);
            else
                f[i][j] = f[i - m][j - 1] + grt[i - m] * po[i - 1];
        }
    double ret = ninf;
    for (unsigned int i = ceil(1.0 * n / m); i <= n; ++i)
        ret = max(ret, f[n][i]);
    return ret;
}
int main()
{
    unsigned int n, m;
    double k;
    cin >> n >> m >> k;
    for (long long* i = val + 1; i < val + 1 + n; ++i)
        cin >> *i;
    init(n, m, k);
    printf("%.2lf\n", dp(n, m));
    return 0;
}