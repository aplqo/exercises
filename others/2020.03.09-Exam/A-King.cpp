#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
#define lowbit(i) ((i) & -(i))
#define bit(i, n) ((1 << (i)) & n)
#define cli(i, n) (~(1 << (i)) & n)
using num_t = unsigned int;
static const int maxn = 9;

static unsigned int lg[(1 << 9) + 10];
static num_t mem[(1 << 9) - 1][maxn + 10][maxn + 10];
static unsigned int row;

inline void GetLog(const unsigned int n)
{
    unsigned int i = 1;
    for (unsigned int t = 0; t <= n; ++t, i <<= 1)
        lg[i] = t;
}
static num_t fun(const unsigned int s, const unsigned int uv, const unsigned int sel, const unsigned int n, const unsigned int k)
{
    if (!n || !k)
        return k == 0;
    if (mem[s][n][k])
        return mem[s][n][k];
    const unsigned int con = s & uv;
    const auto vavid = [con, sel](const unsigned int i) -> bool {
        return (i < 1 || bit(i - 1, sel)) && (i == row - 1 || bit(i + 1, sel)) && bit(i, con) && i < row;
    };
    unsigned int i = 0;
    {
        unsigned int t = con;
        for (i = lg[lowbit(t)]; t && !vavid(i); i = lg[lowbit(t)])
            t -= lowbit(t);
    }
    if (vavid(i))
    {
        const unsigned int msk = (i ? cli(i - 1, s) : s) & (i < row - 1 ? cli(i + 1, s) : s);
        mem[s][n][k] += fun(cli(i, s) & msk, uv, cli(i, sel), n, k - 1);
        mem[s][n][k] += fun(s, cli(i, uv), sel, n, k);
    }
    else
        mem[s][n][k] += fun((1 << row) - 1, s, (1 << row) - 1, n - 1, k);
    return mem[s][n][k];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int k;
    cin >> row >> k;
    GetLog(row);
    cout << fun((1 << row) - 1, (1 << row) - 1, (1 << row) - 1, row, k) << endl;
    return 0;
}
