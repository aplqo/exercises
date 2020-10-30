#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
constexpr unsigned int inf = UINT_MAX / 2;
constexpr int dx[] = { 1, 1, 2, 2, -1, -1, -2, -2 }, dy[] = { 2, -2, 1, -1, 2, -2, 1, -1 };

struct State
{
    inline unsigned int estimate() const;
    inline State move(const unsigned int npos) const;
    inline constexpr bool operator==(const State& r) const { return stat == r.stat && pos == r.pos; }

    unsigned long long stat;
    unsigned int pos;
};
constexpr State finalState { 0b0000010000110001111011111, 12 };
unsigned int nxt[25][8], *cnt[25];

inline unsigned int id(const unsigned int x, const unsigned int y)
{
    return x * 5 + y;
}
static void initNext()
{
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            const unsigned int cid = id(i, j);
            cnt[cid] = nxt[cid];
            for (unsigned int k = 0; k < 8; ++k)
            {
                const int nx = i + dx[k], ny = j + dy[k];
                if (0 <= nx && nx < 5 && 0 <= ny && ny < 5)
                    *(cnt[cid]++) = id(nx, ny);
            }
        }
}

inline unsigned int State::estimate() const { return __builtin_popcount(stat ^ finalState.stat) + (pos != finalState.pos); }
inline State State::move(const unsigned int npos) const
{
    const unsigned long long b = stat & (1 << npos);
    return State { (stat - b) | ((b >> npos) << pos), npos };
}
unsigned int dfs(const State st, const unsigned int dep)
{
    if (st == finalState)
        return 0;
    if (!dep)
        return inf;
    unsigned int ret = inf;
    for (const unsigned int* i = nxt[st.pos]; i < cnt[st.pos]; ++i)
    {
        const State ns = st.move(*i);
        if (ns.estimate() + 1 <= min(ret, dep + 1))
            ret = min(dfs(ns, dep - 1) + 1, ret);
    }
    return ret;
}
unsigned int idaStar(const State ini)
{
    for (unsigned int i = 1; i < 16; i *= 2)
    {
        const unsigned int t = dfs(ini, i);
        if (t != inf)
            return t;
    }
    return dfs(ini, 15);
}
void solve()
{
    State ini {};
    for (unsigned int i = 0; i < 5; ++i)
        for (unsigned int j = 0; j < 5; ++j)
        {
            char s;
            cin >> s;
            switch (s)
            {
            case '0':
                break;
            case '1':
                ini.stat |= 1ull << id(i, j);
                break;
            case '*':
                ini.pos = id(i, j);
            }
        }
    const unsigned int res = idaStar(ini);
    if (res == inf)
        cout << "-1\n";
    else
        cout << res << "\n";
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    initNext();
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}