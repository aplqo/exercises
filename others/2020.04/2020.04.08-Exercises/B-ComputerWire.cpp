/*
* Luogu team T128881 computer wire
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
constexpr int inf = INT_MAX >> 1;
const int maxm = 100000, maxn = 2000;

int sock[maxm + 10], com[maxn + 10];
int f[maxm + 10];

class tree
{
public:
    void build(const unsigned int l, const unsigned int r, tree*(&cur))
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
    int query(const unsigned int l, const unsigned int r)
    {
        return l < r ? this->query_impl(l, r) : inf;
    }
    void modify(const unsigned int p, const int val)
    {
        if (rv - lv == 1)
        {
            this->val = val;
            return;
        }
        if (p < mv)
            lc->modify(p, val);
        else
            rc->modify(p, val);
        this->val = min(lc->val, rc->val);
    }

private:
    int query_impl(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        if (r <= mv)
            return lc->query_impl(l, r);
        else if (l >= mv)
            return rc->query_impl(l, r);
        else
            return min(lc->query_impl(l, mv), rc->query_impl(mv, r));
    }

    tree *lc = nullptr, *rc = nullptr;
    unsigned int lv, rv, mv;
    int val = inf;
} * lt, *gt, tr[maxm * 8 + 10];

int dp(const unsigned int n, const unsigned int m)
{
    {
        tree* c = tr;
        lt = c;
        lt->build(0, m + 1, c);
        gt = c;
        gt->build(0, m + 1, c);
    }
    f[1] = abs(sock[1] - com[1]);
    for (unsigned int i = 2; i <= m; ++i)
        f[i] = min(f[i - 1], abs(com[1] - sock[i]));
    for (unsigned int j = 2; j <= n; ++j)
    {
        for (unsigned int i = 0; i < m; ++i)
        {
            lt->modify(i, f[i] - sock[i + 1]);
            gt->modify(i, f[i] + sock[i + 1]);
        }
        for (unsigned int i = j; i <= m; ++i)
        {
            unsigned int p = lower_bound(sock + j, sock + 1 + i, com[j]) - sock;
            f[i] = min(lt->query(j - 1, p - 1) + com[j], gt->query(p - 1, i) - com[j]);
        }
    }
    return *min_element(f + n, f + m + 1);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> m >> n;
    auto read = [](int& i) -> void { cin >> i; };
    for_each(sock + 1, sock + m + 1, read);
    for_each(com + 1, com + n + 1, read);
    sort(sock + 1, sock + m + 1);
    sort(com + 1, com + 1 + n);
    cout << dp(n, m) << endl;
    return 0;
}