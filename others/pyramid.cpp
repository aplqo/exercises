#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
const int maxn = 3000;
constexpr int ninf = numeric_limits<int>::min() / 2;

struct obj
{
    int w;
    int v;
} o[maxn + 10];
int ans[maxn * 2 + 10][maxn + 10];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i <= n; ++i)
    {
        cin >> o[i].w;
        o[i].v = i;
    }
    for (unsigned int c = 1; c <= n + m; ++c)
    {
        fill(ans[c] + 1, ans[c] + n + 1, ninf);
        for (unsigned int v = 1; v <= n; ++v)
            for (unsigned int i = 0; i <= n; ++i)
                if (v >= o[i].v)
                    ans[c][v] = max({ ans[c - 1][v - o[i].v] + o[i].w, ans[c][v] });
    }
    cout << ans[n + m][n] << endl;
    return 0;
}
