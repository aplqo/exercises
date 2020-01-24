#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
typedef int num;
const int maxn = 30010;

int fa[maxn];
num siz[maxn], dis[maxn];

int GetFather(int x)
{
    if (fa[x] == x)
        return x;
    int f = GetFather(fa[x]);
    dis[x] += dis[fa[x]];
    return fa[x] = f;
}
inline void merge(int from, int to)
{
    int ff = GetFather(from), ft = GetFather(to);
    dis[ff] = siz[ft];
    siz[ft] += siz[ff];
    fa[ff] = ft;
}
inline void init(int x)
{
    iota(fa, fa + x + 1, 0);
    fill(siz, siz + x + 1, 1);
    fill(dis, dis + x + 1, 0);
}

int main()
{
    ios_base::sync_with_stdio(false);
    int p;
    init(maxn - 1);
    cin >> p;
    for (int i = 0; i < p; ++i)
    {
        char op;
        int x;
        cin >> op >> x;
        switch (op)
        {
        case 'M':
            int y;
            cin >> y;
            merge(x, y);
            break;
        case 'C':
            GetFather(x);
            cout << dis[x] << endl;
            break;
        }
    }
    return 0;
}