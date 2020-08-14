#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 15, maxp = 13;
typedef double real;
template <class T>
using mem = T[maxp + 1][maxp + 1][maxp + 1][maxp + 1][5][5];

unsigned int ex[4];
struct State
{
    unsigned int cnt[6];
    unsigned int king[2];
    template <class T>
    inline T& operator[](mem<T> dat) const
    {
        return dat[cnt[0]][cnt[1]][cnt[2]][cnt[3]][king[0]][king[1]];
    }
    template <class T>
    inline T& operator[](mem<T> dat)
    {
        return dat[cnt[0]][cnt[1]][cnt[2]][cnt[3]][king[0]][king[1]];
    }
    inline unsigned int sum() const
    {
        return maxp * 4 - accumulate(cnt, cnt + 4, 0u) + (king[0] == 4) + (king[1] == 4);
    }
    inline State takeNormal(const unsigned int typ) const
    {
        State ret = *this;
        ++ret.cnt[typ];
        return ret;
    }
    inline State takeKing(const unsigned int s, const unsigned int typ) const
    {
        State ret = *this;
        ret.king[s] = typ;
        return ret;
    }
    bool succ() const
    {
        for (unsigned int i = 0; i < 4; ++i)
            if (cnt[i] + (king[0] == i) + (king[1] == i) < ex[i])
                return false;
        return true;
    }
};

real dfs(const State& s)
{
    static mem<bool> vis;
    static mem<real> m;
    if (s.succ())
        return 0;
    if (s[vis])
        return s[m];
    const unsigned int sm = s.sum();
    for (unsigned int i = 0; i < 4; ++i)
        if (s.cnt[i] < maxp)
            s[m] += dfs(s.takeNormal(i)) * (maxp - s.cnt[i]) / sm;
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (s.king[i] != 4)
            continue;
        real t = 1000;
        for (unsigned int j = 0; j < 4; ++j)
            t = min(t, dfs(s.takeKing(i, j)));
        s[m] += t / sm;
    }
    s[m] += 1;
    s[vis] = true;
    return s[m];
}
int main()
{
    unsigned int sum = 0, cnt = 0;
    for (unsigned int i = 0; i < 4; ++i)
    {
        cin >> ex[i];
        sum += ex[i];
        cnt += ex[i] > maxp ? ex[i] - maxp : 0;
    }
    if (cnt > 2 || sum > 54)
    {
        puts("-1.000");
        return 0;
    }
    printf("%.3lf", dfs(State { {}, 4, 4 }));
    return 0;
}