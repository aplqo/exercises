#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 6010;
constexpr unsigned int mod = 2019;

char s1[maxl + 10], s2[maxl + 10];
struct State
{
    unsigned int len, cnt;
    void update(const State& r, const unsigned int delt)
    {
        const unsigned int cl = r.len + delt;
        if (cl < len)
        {
            len = cl;
            cnt = r.cnt;
        }
        else if (cl == len)
            cnt = (cnt + r.cnt) % mod;
    }
};
constexpr State inf { UINT_MAX / 2, 0 };

State dp(const size_t l1, const size_t l2)
{
    static State f[2][maxl + 10];
    for (size_t i = 0; i <= l2; ++i)
        f[0][i] = State { i, 1 };
    for (size_t i = 1; i <= l1; ++i)
    {
        State *const cur = f[i & 0x01], *const lst = f[!(i & 0x01)];
        fill(cur + 1, cur + l2 + 1, inf);
        cur[0] = State { i, 1 };
        for (size_t j = 1; j <= l2; ++j)
        {
            if (s1[i] == s2[j])
                cur[j].update(lst[j - 1], 1);
            else
            {
                cur[j].update(lst[j], 1);
                cur[j].update(cur[j - 1], 1);
            }
        }
    }
    return f[l1 & 0x01][l2];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> (s1 + 1) >> (s2 + 1);
    const State s = dp(strlen(s1 + 1), strlen(s2 + 1));
    cout << s.len << " " << s.cnt << "\n";
    return 0;
}