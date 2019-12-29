#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const int maxn = 35;

struct mem
{
    void print()
    {
        cout << (root + 1) << " ";
        if (l)
            l->print();
        if (r)
            r->print();
    }
    bool vis = false;
    int ret = INT_MIN, root = 0;
    mem *l = nullptr, *r = nullptr;
} ca[maxn][maxn];

int dat[maxn];

int dfs(int l, int r)
{
    mem& c = ca[l][r];
    if (c.vis)
        return c.ret;
    c.vis = true;
    if (r - l == 1)
    {
        c.root = l;
        c.ret = dat[l];
        return dat[l];
    }
    for (int i = l; i < r; ++i)
    {
        int cu = i > l ? dfs(l, i) : 1;
        if (i < r - 1)
            cu *= dfs(i + 1, r);
        cu += dat[i];
        if (cu > c.ret)
        {
            c.root = i;
            c.ret = cu;
            if (i > l)
                c.l = &ca[l][i];
            if (i < r - 1)
                c.r = &ca[i + 1][r];
        }
    }
    return c.ret;
}
int main()
{
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for_each(dat, dat + n, [](int& i) -> void { cin >> i; });
    cout << dfs(0, n) << endl;
    ca[0][n].print();
    return 0;
}