#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;

char str[maxn + 10];
unsigned int sa[maxn + 1], Rank[maxn + 1];

void radixSort(const size_t n, unsigned int rk[], unsigned int tp[])
{
    static unsigned int buk[maxn + 1];
    memset(buk, 0, sizeof(buk));
    for (size_t i = 1; i <= n; ++i)
        ++buk[rk[tp[i]]];
    partial_sum(buk, buk + 1 + maxn, buk);
    for (size_t i = n; i; --i)
        sa[buk[rk[tp[i]]]--] = tp[i];
}
void buildSA(const char str[], const size_t l)
{
    static unsigned int tmp[maxn + 10];
    unsigned int *tp = tmp, *rk = Rank;
    for (size_t i = 1; i <= l; ++i)
    {
        tp[i] = i;
        rk[i] = str[i];
    }
    radixSort(l, rk, tp);
    for (unsigned int i = 1, p = 0; p < l; i <<= 1)
    {
        {
            unsigned int e = 1;
            for (unsigned int j = l + 1 - i; j <= l; ++j)
                tp[e++] = j;
            for (unsigned int j = 1; j <= l; ++j)
                if (sa[j] > i)
                    tp[e++] = sa[j] - i;
        }
        radixSort(l, rk, tp);
        swap(rk, tp);
        rk[sa[1]] = p = 1;
        for (unsigned int j = 2; j <= l && sa[j]; ++j)
            if (tp[sa[j]] == tp[sa[j - 1]] && tp[sa[j] + i] == tp[sa[j - 1] + i])
                rk[sa[j]] = p;
            else
                rk[sa[j]] = ++p;
    }
    for (unsigned int i = 1; i <= l; ++i)
        Rank[sa[i]] = i;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> (str + 1);
    const size_t l = strlen(str + 1);
    buildSA(str, l);
    copy(sa + 1, sa + 1 + l, ostream_iterator<unsigned int>(cout, " "));
    cout << endl;
    return 0;
}