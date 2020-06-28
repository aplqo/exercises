#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
using namespace std;
const unsigned int maxn = 1000, maxl = 11;

template <class T = unsigned int>
[[nodiscard]] static T read()
{
    T ret = 0;
    char c = getchar();
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = getchar();
    }
    return ret;
}
struct st
{
    void init(const unsigned long long dat[], const unsigned int l)
    {
        for (unsigned int i = 0; i < l; ++i)
            tab[i][0] = dat[i];
        for (unsigned int i = 1; i <= lg[l]; ++i)
            for (unsigned int j = 0; j + (1 << i) <= l; ++j)
                tab[j][i] = max(tab[j][i - 1], tab[j + (1 << (i - 1))][i - 1]);
    }
    inline unsigned long long query(const unsigned int l, const unsigned int r) const
    {
        const unsigned int d = lg[r - l + 1];
        return max(tab[l][d], tab[r + 1 - (1 << d)][d]);
    }

    static void getLog(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;
    }

    unsigned long long tab[maxn + 2][maxl + 1];

private:
    static unsigned int lg[maxn + 10];
} table[maxn + 1];
unsigned int st::lg[maxn + 10];
unsigned int val[maxn + 2][maxn + 2];
unsigned long long fu[maxn + 2][maxn + 2], fd[maxn + 2][maxn + 2], f[maxn + 2][maxn + 2];

static void dpUp(const unsigned int n)
{
    for (unsigned int i = n; i > 0; --i)
        for (unsigned int j = 1; j <= i; ++j)
        {
            fu[i - 1][j] = max(fu[i - 1][j], fu[i][j] + val[i][j]);
            fu[i - 1][j - 1] = max(fu[i - 1][j - 1], fu[i][j] + val[i][j]);
        }
}
static void dpDown(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= i; ++j)
        {
            fd[i + 1][j] = max(fd[i + 1][j], fd[i][j] + val[i][j]);
            fd[i + 1][j + 1] = max(fd[i + 1][j + 1], fd[i][j] + val[i][j]);
        }
}
static void init(const unsigned int n)
{
    dpUp(n);
    dpDown(n);
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (unsigned int j = 1; j <= i; ++j)
            f[i][j] = fu[i][j] + fd[i][j] + val[i][j];
        table[i].init(f[i], i + 2);
    }
}
inline unsigned long long query(const unsigned int l, const unsigned int r)
{
    return max(table[l].query(0, r - 1), table[l].query(r + 1, l + 1));
}

int main()
{
    const unsigned int n = read();
    const auto q = read();
    for (unsigned int i = 1; i <= n; ++i)
        generate_n(val[i] + 1, i, read<unsigned int>);
    st::getLog(n + 3);
    init(n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const auto x = read();
        const auto y = read();
        if (x == 1 && y == 1)
            printf("-1\n");
        else
            printf("%llu\n", query(x, y));
    }
    return 0;
}