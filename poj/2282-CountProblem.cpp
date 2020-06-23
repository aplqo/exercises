#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
typedef unsigned int num_t;
const unsigned int maxl = 9;

num_t po[maxl + 10] = { 1 };
unsigned int ans[2][10];
unsigned int f[maxl + 3] = { 0, 0 };

inline void init(const unsigned int l)
{
    for (unsigned int i = 1; i <= l; ++i)
        po[i] = po[i - 1] * 10;
    for (unsigned int i = 1; i <= l; ++i)
        f[i] = po[i] - 1;
}
inline void solve(num_t x, unsigned int dat[10])
{
    unsigned int digit = 0, rat = 0;
    while (x)
    {
        const unsigned int i = x % 10;
        dat[i] += rat + (i ? 1 : 0);
        for (unsigned int t = 1; t < i; ++t)
            dat[t] += po[digit];
        if (digit)
        {
            for (unsigned int t = 0; t < 10; ++t)
                dat[t] += digit * i * po[digit - 1];
            if (!i)
                dat[0] -= f[digit];
        }
        rat += i * po[digit];
        ++digit;
        x /= 10;
    }
}

int main()
{
    init(maxl + 2);
    while (true)
    {
        num_t a, b;
        cin >> a >> b;
        if (!a && !b)
            break;
        if (a > b)
            swap(a, b);
        memset(ans, 0, sizeof(ans));
        solve(a - 1, ans[0]);
        solve(b, ans[1]);
        for (unsigned int i = 0; i < 9; ++i)
            cout << ans[1][i] - ans[0][i] << " ";
        cout << ans[1][9] - ans[0][9] << endl;
    }
    return 0;
}