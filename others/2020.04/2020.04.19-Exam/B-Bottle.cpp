/*Luogu team T129702:bottle*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
#define lowbit(x) ((x) & -(x))
const int maxn = 100000;

unsigned int tree[maxn + 10], n;
unsigned int gt[maxn + 10], lt[maxn + 10];
unsigned long siz[maxn + 10], tmp[maxn + 10];

inline void modify(unsigned int pos, unsigned int val)
{
    for (; pos <= n; pos += lowbit(pos))
        tree[pos] = max(tree[pos], val);
}
inline unsigned int query(unsigned int pos)
{
    unsigned int ret = 0;
    for (; pos; pos -= lowbit(pos))
        ret = max(ret, tree[pos]);
    return ret;
}

void discretize()
{
    copy(siz + 1, siz + 1 + n, tmp + 1);
    sort(tmp + 1, tmp + 1 + n);
    unsigned long* en = unique(tmp + 1, tmp + 1 + n);
    for (unsigned int i = 1; i <= n; ++i)
        siz[i] = lower_bound(tmp + 1, en, siz[i]) - tmp;
}
void lis()
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        gt[i] = query(siz[i]) + 1;
        modify(siz[i], gt[i]);
    }
}
void lds()
{
    fill(tree, tree + 1 + n, 0);
    for (unsigned int i = 1; i <= n; ++i)
    {
        lt[i] = query(n - siz[i] + 1) + 1;
        modify(n - siz[i] + 1, lt[i]);
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    for (unsigned long* i = siz + 1; i < siz + 1 + n; ++i)
        cin >> *i;
    reverse(siz + 1, siz + 1 + n);
    discretize();
    lis();
    lds();
    unsigned int ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ans = max(ans, gt[i] + lt[i] - 1);
    cout << ans << endl;
    return 0;
}