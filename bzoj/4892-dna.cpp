#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;
#define adv(it, dis) ((it) + (dis)-1)
#define dis(fr, to) ((to) - (fr) + 1)
typedef uint_fast64_t hash_t;
const hash_t base = 131;
const int maxl = 1e5;

hash_t hs[2][maxl + 10], po[maxl + 10] = { 1 };
char s[maxl + 10], s0[maxl + 10];
size_t ls, ls0;

inline void getPow(unsigned int v)
{
    for (unsigned int i = 1; i <= v; ++i)
        po[i] = po[i - 1] * base;
}
inline hash_t getSub(hash_t h[maxl + 10], unsigned int beg, unsigned int len)
{
    return h[beg + len - 1] - h[beg - 1] * po[len];
}
void getHash(const char str[], size_t len, hash_t h[maxl + 10])
{
    for (unsigned int i = 1; i <= len; ++i)
        h[i] = h[i - 1] * base + str[i] - 'A';
}

void solve()
{
    unsigned int ans = 0;
    cin >> (s + 1) >> (s0 + 1);
    getHash(s, ls = strlen(s + 1), hs[0]);
    getHash(s0, ls0 = strlen(s0 + 1), hs[1]);
    for (unsigned int i = 1; i <= ls - ls0 + 1; ++i)
    {
        if (getSub(hs[0], i, ls0) == getSub(hs[1], 1, ls0))
        {
            ++ans;
            continue;
        }
        unsigned int lst = 1;
        for (unsigned int j = 0; j < 3; ++j)
        {
            unsigned int l = lst, r = ls0;
            while (l <= r)
            {
                unsigned int m = (l + r) >> 1;
                if (getSub(hs[0], adv(i, lst), dis(lst, m)) == getSub(hs[1], adv(1, lst), dis(lst, m)))
                    l = m + 1;
                else
                    r = m - 1;
            }
            lst = l + 1;
        }
        if (lst <= ls0 && getSub(hs[0], adv(i, lst), dis(lst, ls0)) != getSub(hs[1], adv(1, lst), dis(lst, ls0)))
            continue;
        ++ans;
    }
    cout << ans << endl;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    getPow(maxl);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i)
        solve();
    return 0;
}