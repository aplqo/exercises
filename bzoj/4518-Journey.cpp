#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 3000;
using num = long long;
using real = double;

num f[2][maxn + 1], s[maxn + 1];
#define X(k) (s[k])
#define K(i) (2 * s[i])
#define Y(k, t) (f[t][k] + s[k] * s[k])
#define C(i) (s[i] * s[i])

struct point
{
    point(num x, num y)
        : x(x)
        , y(y)
    {
    }
    point(const unsigned int i, const unsigned int t)
        : point(X(i), Y(i, t))
    {
    }

    num x, y;
};

inline real slope(const point a, const point b)
{
    return static_cast<real>(b.y - a.y) / (b.x - a.x);
}
num dp(const unsigned int n, const unsigned int m)
{
    static unsigned int q[2][maxn + 1], *front[2] = { q[0], q[1] }, *back[2] = { q[0] + 1, q[1] };
    for (unsigned int i = 1; i <= m; ++i)
    {
        const unsigned int cur = i & 0x01, lst = (i - 1) & 0x01;
        front[cur] = back[cur] = q[cur];
        q[cur][0] = 0;
        ++back[cur];
        for (unsigned int j = 1; j <= n; ++j)
        {
            while (back[lst] - front[lst] >= 2 && slope(point(*(front[lst] + 1), lst), point(*front[lst], lst)) < K(j))
                ++front[lst];
            f[cur][j] = Y(*front[lst], lst) - K(j) * X(*front[lst]) + C(j);
            while (back[cur] - front[cur] >= 2 && slope(point(*(back[cur] - 1), cur), point(j, cur)) < slope(point(*(back[cur] - 2), cur), point(j, cur)))
                --back[cur];
            *(back[cur]++) = j;
        }
    }
    return f[m & 0x01][n] * m - s[n] * s[n];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<num>(cin), n, s + 1);
    partial_sum(s + 1, s + 1 + n, s + 1);
    cout << dp(n, m) << endl;
    return 0;
}