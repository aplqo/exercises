#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1e6;

unsigned int mat[maxn + 10];

void greedy(const int n, const int m)
{
    int cb = m;
    for (int i = n - 1; i >= 0;)
    {
        const int oldb = cb;
        while ((cb & i) != i)
            ++cb;
        ++cb;
        for (int t = cb - oldb, tp = cb; t > 0; --t)
            mat[i--] = --tp;
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    greedy(n, m);
    for (unsigned int i = 0; i < n; ++i)
        cout << i << " " << mat[i] << "\n";
    return 0;
}