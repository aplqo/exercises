/*Luogu team T132587: UOI disk*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
using std::getchar;
using std::min;
using std::printf;
const int maxn = 1e6, maxl = 25;

unsigned int tab[maxn + 10][maxl + 10];
unsigned int val[maxn + 10], lg[maxn + 10];

template <class T>
void read(T& dat)
{
    char c = getchar();
    dat = 0;
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        dat = dat * 10 + c - '0';
        c = getchar();
    }
}
void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
void init(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        tab[i][0] = val[i];
    for (unsigned int i = 1; i <= lg[n]; ++i)
        for (unsigned int j = 0; j + (1 << i) <= n; ++j)
            tab[j][i] = min(tab[j][i - 1], tab[j + (1 << (i - 1))][i - 1]);
}
unsigned int query(const unsigned int l, const unsigned int r)
{
    const unsigned int d = lg[r - l + 1];
    return min(tab[l][d], tab[r - (1 << d) + 1][d]);
}
unsigned int find(int a, int b, const unsigned int v)
{
    if (query(a, b) > v)
        return 0;
    while (a <= b)
    {
        const int m = (a + b) >> 1;
        if (query(a, m) <= v)
            b = m - 1;
        else
            a = m + 1;
    }
    return a + 1;
}
int main()
{
    unsigned int n, m;
    read(n);
    read(m);
    for (unsigned int* i = val; i < val + n; ++i)
        read(*i);
    getLog(n);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b, c;
        int ans;
        read(a);
        read(b);
        read(c);
        ans = find(a - 1, b - 1, c);
        printf("%d\n", (ans ? ans : -1));
    }
    return 0;
}
