#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5, maxm = 3e5, maxw = 3;

class DynArray
{
public:
    inline void push_back(const unsigned long long v)
    {
        *(ptr++) = v;
        ++size;
    }
    inline unsigned long long operator[](const unsigned int idx) const
    {
        return val[idx];
    }
    unsigned int size;
    unsigned long long val[maxm + 10] = {}, *ptr = val;

} obj[maxw + 10];

unsigned long long dp(const unsigned int m)
{
    static unsigned long long f[maxm + 10];
    static unsigned int cnt[maxm + 10][4];
    for (unsigned int i = 1; i < 4; ++i)
        sort(obj[i].val, obj[i].ptr, greater<unsigned long long>());
    for (unsigned int i = 1; i <= m; ++i)
    {
        unsigned int from = 0;
        for (unsigned int k = 1; k < 4 && k <= i; ++k)
        {
            const unsigned long long prof = f[i - k] + obj[k][cnt[i - k][k]];
            if (prof > f[i])
            {
                f[i] = prof;
                from = k;
            }
        }
        if (from)
        {
            copy(cnt[i - from], cnt[i - from] + 4, cnt[i]);
            ++cnt[i][from];
        }
    }
    return *max_element(f, f + m + 1);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int w;
        unsigned long long p;
        cin >> w >> p;
        obj[w].push_back(p);
    }
    cout << dp(m) << "\n";
    return 0;
}