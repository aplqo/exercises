#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const int maxn = 10000, maxm = 5, maxs = (1 << (maxm * 2));
constexpr num_t mod = 1e6;

unsigned int sta[maxs + 10], scur;
unsigned int orig, msk;
num_t f[maxn + 10][maxs + 10];

inline bool isSame(const unsigned int cur, const unsigned int lst)
{
    static constexpr unsigned int mask1 = 0xaaaaaaaa, mask2 = 0x55555555;
    const unsigned int b1 = (cur & mask1) ^ (lst & mask1);
    const unsigned int b2 = (cur & mask2) ^ (lst & mask2);
    return (b1 | (b2 << 1)) ^ (mask1 & msk);
}
void GetState(const unsigned int m, const unsigned int pos = 1)
{
    static unsigned int tmp[maxm + 10];
    if (pos > m)
    {
        for (unsigned int i = 1; i <= m; ++i)
            sta[scur] |= tmp[i] << (2 * (i - 1));
        ++scur;
        return;
    }
    for (unsigned int i = 1; i <= 3; ++i)
        if (i != tmp[pos - 1])
        {
            tmp[pos] = i;
            GetState(m, pos + 1);
        }
}
template <class T>
inline num_t dp(const unsigned int p, const unsigned int end)
{
    T nxt;
    for (unsigned int i = p; i != end; i = nxt(i, 1))
    {
        for (unsigned int j = 0; j < scur; ++j)
        {
            for (unsigned int k = 0; k < scur; ++k)
            {
                if (isSame(sta[k], sta[j]))
                    continue;
                f[nxt(i, 1)][k] = (f[nxt(i, 1)][k] + f[i][j]) % mod;
            }
        }
    }
    num_t ret = 0;
    for (unsigned int i = 0; i < scur; ++i)
        ret = (ret + f[end][i]) % mod;
    return ret;
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, m, k;
    num_t ans = 1;
    cin >> n >> m >> k;
    for (unsigned int i = 0; i < m; ++i)
    {
        static unsigned int tmp = 0;
        unsigned int t;
        cin >> t;
        if (t == tmp)
        {
            cout << "0" << endl;
            return 0;
        }
        tmp = t;
        orig |= t << (i * 2);
    }

    msk = (1 << (m * 2)) - 1;
    GetState(m);
    f[k][find(sta, sta + scur, orig) - sta] = 1;

    ans *= dp<plus<unsigned int>>(k, n);
    ans = (ans * dp<minus<unsigned int>>(k, 1)) % mod;
    cout << ans << endl;
    return 0;
}