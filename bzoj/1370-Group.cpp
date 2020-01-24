#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1010;

int fa[maxn * 2], siz[maxn * 2];
int n;

int GetFather(int x)
{
    return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline void merge(int x, int y)
{
    int fx = GetFather(x), fy = GetFather(y);
    if (fx == fy)
        return;
    if (siz[fx] < siz[fy])
        swap(fx, fy);
    fa[fy] = fx;
    siz[fx] += siz[fy];
}
void iota(int* l, int* r, int init)
{
    for (; l < r; ++l, ++init)
        *l = init;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int m;
    cin >> n >> m;
    iota(fa, fa + n * 2 + 1, 0);
    fill(siz, siz + n * 2 + 1, 1);
    for (int i = 0; i < m; ++i)
    {
        char op;
        int x, y;
        cin >> op >> x >> y;
        switch (op)
        {
        case 'E':
            merge(x, y + n);
            merge(y, x + n);
            break;
        case 'F':
            merge(x, y);
            break;
        }
    }
    for (int i = 1; i <= 2 * n; ++i)
        GetFather(i);
    sort(fa + 1, fa + 1 + n);
    cout << (unique(fa + 1, fa + 1 + n) - (fa + 1)) << endl;
    return 0;
}