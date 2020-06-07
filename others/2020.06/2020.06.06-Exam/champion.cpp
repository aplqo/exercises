#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
using namespace std;
const int maxn = 500;

unsigned int cake[maxn + 5][maxn + 5], s[maxn + 5][maxn + 5];
unsigned int n, m, a, b;

void init()
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + cake[i][j];
}
inline unsigned int sum(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
    return s[x2][y2] + s[x1 - 1][y1 - 1] - s[x1 - 1][y2] - s[x2][y1 - 1];
}
bool test(const unsigned int val)
{
    unsigned int cntC = 0, pc = 1;
    for (unsigned int i = 1; i <= n && pc <= n; ++i)
    {
        unsigned int pr = 1, cntR = 1;
        for (unsigned int j = 1; j < m && pr <= m; ++j)
            if (sum(pc, pr, i, j) >= val)
            {
                ++cntR;
                pr = j + 1;
            }
        if (cntR < b || (cntR == b && pr <= m && sum(pc, pr, i, m) < val))
            continue;
        pc = i + 1;
        ++cntC;
    }
    return cntC >= a;
}
unsigned int solve()
{
    unsigned int l = 0, r = sum(1, 1, n, m);
    while (l <= r)
    {
        const unsigned int m = (l + r) >> 1;
        if (test(m))
            l = m + 1;
        else
            r = m - 1;
    }
    return r;
}

int main()
{
    scanf("%u%u%u%u", &n, &m, &a, &b);
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
            scanf("%u", cake[i] + j);
    init();
    printf("%d", solve());
    return 0;
}
