#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;

char str[maxn + 10];
unsigned int sa[maxn + 10], Rank[maxn + 10], height[maxn + 10];

void radixSort(const size_t len, const unsigned int rk[], const unsigned int tp[])
{
    static unsigned int buk[maxn + 10];
    memset(buk, 0, sizeof(buk));
    for (unsigned int i = 1; i <= len; ++i)
        ++buk[rk[tp[i]]];
    partial_sum(buk + 1, buk + 1 + max<size_t>(26, len), buk + 1);
    for (unsigned int i = len; i; --i)
        sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const char str[], const size_t len)
{
    static unsigned int tmp[maxn + 10];
    unsigned int *rk = Rank, *tp = tmp;
    for (unsigned int i = 1; i <= len; ++i)
    {
        rk[i] = str[i] + 1 - 'a';
        tp[i] = i;
    }
    radixSort(len, rk, tp);
    for (unsigned int i = 1, p = 0; p < len; i <<= 1)
    {
        {
            unsigned int c = 0;
            for (unsigned int j = len + 1 - i; j <= len; ++j)
                tp[++c] = j;
            for (unsigned int j = 1; j <= len; ++j)
                if (sa[j] > i)
                    tp[++c] = sa[j] - i;
        }
        radixSort(len, rk, tp);
        swap(rk, tp);
        rk[sa[1]] = p = 1;
        for (unsigned int j = 2; j <= len; ++j)
            if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
                rk[sa[j]] = p;
            else
                rk[sa[j]] = ++p;
    }
    for (unsigned int i = 1; i <= len; ++i)
        Rank[sa[i]] = i;
    for (unsigned int i = 1, k = 0; i <= len; ++i)
    {
        if (k)
            --k;
        while (str[i + k] == str[sa[rk[i] - 1] + k])
            ++k;
        height[i] = k;
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n;
    cin >> n >> (str + 1);
    buildSA(str, n);
    cout << (n * (n + 1) / 2 - accumulate(height, height + n + 1, 0ull)) << "\n";
    return 0;
}