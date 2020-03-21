#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;
const int maxn = 100000;
constexpr unsigned long long maxs1 = 100000;

unsigned long long store[maxn + 10];
unsigned long long f[maxs1 + 10];

template <class T>
inline void read(T& dat)
{
    dat = 0;
    char c = getchar();
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        dat = dat * 10 + c - '0';
        c = getchar();
    }
}
inline unsigned long long dp(const unsigned int n, const unsigned long long s)
{
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned long long j = store[i]; j <= s; ++j)
            f[j] = max(f[j], f[j - store[i]] + 1);
    return f[s];
}
int main()
{
    unsigned int n;
    unsigned long long s, ans = 0, base = 0;
    read(n);
    read(s);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned long long a, b;
        read(a);
        read(b);
        if (a <= b)
        {
            cout << "-1" << endl;
            return 0;
        }
        store[i] = a - b;
    }
    cout << max(dp(n, s) + base, ans) << endl;
    return 0;
}