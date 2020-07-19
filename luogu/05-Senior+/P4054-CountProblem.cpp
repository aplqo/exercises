#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 300, maxc = 100;

unsigned int n, m;
class FTree
{
public:
#define lowbit(x) ((x) & -(x))
    void modify(unsigned int x, const unsigned int y, const int v)
    {
        for (; x <= n; x += lowbit(x))
            for (unsigned int j = y; j <= m; j += lowbit(j))
                dat[x][j] += v;
    }
    inline int sum(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2) const
    {
        return query(x2, y2) + query(x1 - 1, y1 - 1) - query(x1 - 1, y2) - query(x2, y1 - 1);
    }

private:
    int query(unsigned int x, const unsigned int y) const
    {
        int ret = 0;
        for (; x; x -= lowbit(x))
            for (unsigned int j = y; j; j -= lowbit(j))
                ret += dat[x][j];
        return ret;
    }
    int dat[maxn + 1][maxn + 1] = {};
} tr[maxc + 1];
int val[maxn + 1][maxn + 1];
inline void change(const unsigned int x, const unsigned int y, const int v)
{
    tr[val[x][y]].modify(x, y, -1);
    tr[v].modify(x, y, 1);
    val[x][y] = v;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int q;
    cin >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= m; ++j)
        {
            cin >> val[i][j];
            tr[val[i][j]].modify(i, j, 1);
        }
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int op;
        cin >> op;
        if (op == 1)
        {
            unsigned int x, y, c;
            cin >> x >> y >> c;
            change(x, y, c);
        }
        else
        {
            unsigned int x1, y1, x2, y2, c;
            cin >> x1 >> x2 >> y1 >> y2 >> c;
            cout << tr[c].sum(x1, y1, x2, y2) << endl;
        }
    }
    return 0;
}