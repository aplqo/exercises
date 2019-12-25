#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 30005;

int fa[maxn], dis[maxn], siz[maxn];

int GetFather(int x)
{
    if (fa[x] == x)
        return x;
    int p = fa[x];
    fa[x] = GetFather(fa[x]);
    dis[x] += dis[p];
    return fa[x];
}
int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    fill(siz, siz + maxn + 1, 1);
    iota(fa + 1, fa + 1 + maxn, 1);
    for (int i = 0; i < t; ++i)
    {
        char op;
        int x, y, fx, fy;
        cin >> op >> x >> y;
        fx = GetFather(x);
        fy = GetFather(y);
        if (op == 'M')
        {
            dis[fx] = siz[fy];
            siz[fy] += siz[fx];
            fa[fx] = fy;
        }
        else
        {
            if (fx != fy)
                cout << "-1" << endl;
            else
                cout << (abs(dis[x] - dis[y]) - 1) << endl;
        }
    }
    return 0;
}
