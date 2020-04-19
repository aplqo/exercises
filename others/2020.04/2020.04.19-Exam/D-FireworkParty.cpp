/*Luogu team T129707: firework party*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100000;

unsigned int n = maxn, cnt = 0;
class stree
{
public:
    void build(const unsigned int l, const unsigned int r, stree*(&cur))
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        if (r - l == 1)
            return;
        lc = ++cur;
        lc->build(lv, mv, cur);
        rc = ++cur;
        rc->build(mv, rv, cur);
    }
    void modify(const unsigned int pos, const unsigned int val)
    {
        if (rv - lv == 1)
        {
            this->val = val;
            cnt = val > 0 ? 1 : 0;
            return;
        }
        if (pos < mv)
            lc->modify(pos, val);
        else if (pos >= mv)
            rc->modify(pos, val);
        cnt = lc->cnt + rc->cnt;
        this->val = max(lc->val, rc->val);
    }
    unsigned int query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        if (!cnt)
            return 0;
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return max(lc->query(l, mv), rc->query(mv, r));
    }

private:
    unsigned int lv, rv, mv;
    stree *lc, *rc;
    unsigned int val, cnt = 0;
} node[maxn * 4 + 10], *tr = node;
namespace treea
{
#define lowbit(x) ((x) & -(x))

    int tree[maxn + 10];
    void modify(unsigned int pos, const int val)
    {
        for (; pos <= n; pos += lowbit(pos))
            tree[pos] += val;
    }
    unsigned int query(unsigned int pos)
    {
        unsigned int ret = 0;
        for (; pos; pos -= lowbit(pos))
            ret += tree[pos];
        return ret;
    }
    unsigned int find(const unsigned int pos)
    {
        unsigned int l = 1, r = cnt;
        while (l <= r)
        {
            const unsigned int m = (l + r) >> 1;
            if (query(m) < pos)
                l = m + 1;
            else
                r = m - 1;
        }
        return l;
    }

#undef lowbit
}

void add(const unsigned int val)
{
    treea::modify(cnt + 1, 1);
    tr->modify(cnt + 1, val);
    ++cnt;
}
void remove(const unsigned int pos)
{
    unsigned int p = treea::find(pos);
    treea::modify(p, -1);
    tr->modify(p, 0);
}
unsigned int query(const unsigned int l, const unsigned int r)
{
    const unsigned int lp = treea::find(l), rp = treea::find(r);
    return tr->query(lp, rp + 1);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int ans = 0;
    {
        stree* c = node;
        tr->build(1, maxn, c);
    }
    while (true)
    {
        int o;
        unsigned int l, r, a;
        cin >> o;
        switch (o)
        {
        case 0:
            cin >> l >> r;
            ans = query(l, r);
            cout << ans << endl;
            break;
        case 1:
            cin >> a;
            remove(a);
            break;
        case 2:
            cin >> a;
            add(ans ^ a);
            break;
        default:
            return 0;
        }
    }
    return 0;
}