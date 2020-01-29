#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
#define lowbit(x) ((x) & -(x))
const int maxn = 8000;

int ans[maxn + 10], a[maxn + 10];
int tree[maxn + 10];
int n;

class sel
{
public:
    void init(int n)
    {
        for (int i = 1; i <= n; ++i)
            fa[i] = i;
    }
    int GetNxt(int x)
    {
        return fa[x] == x ? x : fa[x] = GetNxt(fa[x]);
    }
    void SetNxt(int from, int to)
    {
        int f = GetNxt(to);
        fa[from] = f;
    }

private:
    int fa[maxn + 10];
} llst, rlst;

inline void update(int pos, int val)
{
    for (; pos <= n; pos += lowbit(pos))
        tree[pos] += val;
}
inline int query(int pos)
{
    int ret = 0;
    for (; pos; pos -= lowbit(pos))
        ret += tree[pos];
    return ret;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n;
    for (int* i = a + 2; i < a + n + 1; ++i)
        cin >> *i;
    for (int i = 1; i <= n; ++i)
        update(i, 1);
    llst.init(n);
    rlst.init(n);

    for (int i = n; i; --i)
    {
        int l = 1, r = n;
        while (l <= r)
        {
            int mid = (l + r) >> 1, v = query(mid) - 1;
            if (v > a[i])
                r = mid - 1;
            else
                l = mid + 1;
        }

        int lf = llst.GetNxt(r), rf = rlst.GetNxt(r);
        if (lf > 0 && query(lf) - 1 == a[i])
            r = lf;
        else
            r = rf;
        ans[i] = r;
        llst.SetNxt(r, r - 1);
        rlst.SetNxt(r, r + 1);
        update(r, -1);
    }

    for (int i = 1; i <= n; ++i)
        cout << ans[i] << endl;
    return 0;
}