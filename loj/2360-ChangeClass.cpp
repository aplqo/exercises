#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <limits>
using namespace std;
using real_t = double;
using integer_t = unsigned int;
const char fmt[] = "%.2lf\n";
constexpr int maxn = 2000, maxv = 300, maxm = 2000;

integer_t dis[maxv + 10][maxv + 10];
unsigned int clas[2][maxn + 10];
real_t k[maxn + 10], f[maxn + 10][maxm + 10][2];

inline void init(const unsigned int v)
{
    static constexpr integer_t inf = numeric_limits<integer_t>::max() / 2;
    for (unsigned int i = 1; i <= v; ++i)
        for (unsigned int j = 1; j <= v; ++j)
            dis[i][j] = (i == j ? 0 : inf);
}
inline void floyd(const unsigned int v)
{
    for (unsigned int k = 1; k <= v; ++k)
        for (unsigned int i = 1; i <= v; ++i)
            for (unsigned int j = i + 1; j <= v; ++j)
            {
                dis[i][j] = min(dis[i][j], integer_t(dis[i][k] + dis[k][j]));
                dis[j][i] = dis[i][j];
            }
}
inline void dp(const unsigned int n, const unsigned int m)
{
    static constexpr real_t inf = numeric_limits<real_t>::max() / 2;
    const static auto Distance = [](unsigned int i, bool ls, bool ts) -> integer_t {
        return dis[clas[ls][i - 1]][clas[ts][i]];
    };
    const static auto Expect = [](unsigned int i, function<real_t(bool)> f) -> real_t {
        return k[i] * f(true) + (1 - k[i]) * f(false);
    };
    const static auto Value = [](unsigned int i, unsigned int j, bool ls, bool ts) -> real_t {
        return f[i - 1][j][ls] + (ls ? Expect(i - 1, bind(Distance, i, placeholders::_1, ts)) : Distance(i, ls, ts));
    };

    for (unsigned int i = 1; i <= n; ++i)
    {
        f[i][0][0] = f[i - 1][0][0] + Distance(i, false, false);
        f[i][0][1] = inf;
    }
    for (unsigned int j = 1; j <= m; ++j)
        for (unsigned int i = 1; i <= n; ++i)
        {
            f[i][j][0] = min(Value(i, j, false, false), Value(i, j, true, false));
            f[i][j][1] = min(Expect(i, bind(Value, i, j - 1, true, placeholders::_1)), Expect(i, bind(Value, i, j - 1, false, placeholders::_1)));
        }
}

int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, m, v, e;
    cin >> n >> m >> v >> e;
    init(v);
    for_each(clas[0] + 1, clas[0] + 1 + n, [](unsigned int& i) -> void { cin >> i; });
    for_each(clas[1] + 1, clas[1] + 1 + n, [](unsigned int& i) -> void { cin >> i; });
    for_each(k + 1, k + 1 + n, [](real_t& i) -> void { cin >> i; });
    for (unsigned int i = 0; i < e; ++i)
    {
        unsigned int a, b;
        integer_t w;
        cin >> a >> b >> w;
        dis[a][b] = min(dis[a][b], w);
        dis[b][a] = dis[a][b];
    }

    floyd(v);
    dp(n, m);

    printf(fmt, round(min(f[n][m][0], f[n][m][1]) * 100.0) / 100.0);
    return 0;
}