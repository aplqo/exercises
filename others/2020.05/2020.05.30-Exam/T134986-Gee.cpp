#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1024, maxm = 1000;

char gene[maxn][maxm + 10];
char root[maxm + 10];
unsigned int cnt[26];

char findMax(const unsigned int n, const unsigned int pos)
{
    memset(cnt, 0, sizeof(cnt));
    for (unsigned int i = 0; i < n; ++i)
        ++cnt[gene[i][pos] - 'A'];
    char ret;
    unsigned int mv = 0;
    for (unsigned int i = 0; i < 26; ++i)
        if (cnt[i] > mv)
        {
            mv = cnt[i];
            ret = 'A' + i;
        }
    return ret;
}
unsigned int solve(const unsigned int n, const unsigned int pos)
{
    char tar;
    tar = root[pos] = findMax(n, pos);
    unsigned int ret = 0;
    for (unsigned int nxt = 1; nxt < n; nxt <<= 1)
    {
        const unsigned int step = nxt << 1;
        for (unsigned int i = 0; i + nxt < n; i += step)
        {
            const char ano = gene[i + nxt][pos], cur = gene[i][pos];
            if (cur == ano)
                continue;
            else if (cur == tar || ano == tar)
            {
                ++ret;
                gene[i][pos] = tar;
            }
            else
            {
                ++ret;
                gene[i][pos] = cnt[ano] > cnt[cur] ? ano : cur;
            }
        }
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
        cin >> gene[i];
    unsigned int v = 0;
    for (unsigned int i = 0; i < m; ++i)
        v += solve(n, i);
    root[m] = '\0';
    cout << root << " " << v << endl;
    return 0;
}