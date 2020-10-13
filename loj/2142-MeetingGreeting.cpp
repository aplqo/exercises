#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 5e4, maxl = 16, maxb = 1u << maxl;
const unsigned int maxu = maxl, maxp = INT_MAX; // max update times, max pow process

unsigned int mod, c;
class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
    void pow(const unsigned int l, const unsigned int r);
    unsigned int sum(const unsigned int l, const unsigned int r) const;

private:
    inline void pushup();

    SegmentTree *lc, *rc;
    unsigned int lv, mv, rv;
    unsigned int val;
    bool unchanged;
} snodes[maxn * 4 + 10], *root = snodes;
class QuickPow
{
public:
    void init(const unsigned long long c, const unsigned int mod, const unsigned int siz);
    unsigned int pow(const unsigned int p) const;

private:
    unsigned int blk, mod;
    unsigned int block[maxb + 10], step[maxb + 10];
} memp[maxl + 10];
unsigned int a[maxn + 10], phiMod[maxn + 10], pcur = 0;
unsigned int pows[maxn + 10][maxl + 10], counter[maxn + 10];

unsigned int lst[maxn + 10];

bool exceed[maxn + 10];

void QuickPow::init(const unsigned long long c, const unsigned int mod, const unsigned int siz)
{
    blk = sqrt(siz);
    this->mod = mod;
    step[0] = block[0] = 1;
    for (unsigned int i = 1; i < blk; ++i)
        step[i] = (step[i - 1] * c) % mod;
    const unsigned long long stp = (step[blk - 1] * c) % mod;
    for (unsigned int i = 1; i <= blk + 1; ++i)
        block[i] = (block[i - 1] * stp) % mod;
}
unsigned int QuickPow::pow(const unsigned int p) const
{
    return (block[p / blk] * static_cast<unsigned long long>(step[p % blk])) % mod;
}
unsigned int quickPow(unsigned long long a, unsigned int e)
{
    unsigned int ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = ret * a;
        a = a * a;
    }
    return ret;
}
bool checkExceed(unsigned long long a, unsigned int e)
{
    if (e > 64)
        return true;
    constexpr unsigned long long lim = INT_MAX;
    unsigned long long ret = 1;
    for (; e && ret < lim && a < lim; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret >= lim;
}
unsigned int phi(unsigned int x)
{
    if (x < 2)
        return 0;
    unsigned int ret = x;
    for (unsigned int i = 2; i * i <= x; ++i)
    {
        if (x % i)
            continue;
        ret = (ret / i) * (i - 1);
        while ((x % i) == 0)
            x /= i;
    }
    if (x > 1)
        ret = (ret / x) * (x - 1);
    return ret;
}
void phiChain(const unsigned int x)
{
    phiMod[pcur++] = x;
    while (phiMod[pcur - 1])
    {
        phiMod[pcur] = phi(phiMod[pcur - 1]);
        ++pcur;
    }
    --pcur;
    phiMod[pcur] = 1;
}
static void init(const unsigned int n)
{
    phiChain(mod);
    for (unsigned int i = 0; i < pcur; ++i)
        memp[i].init(c, phiMod[i], maxp);
    {
        SegmentTree* c = snodes;
        root->build(1, n + 1, c);
    }
}
void powC(const unsigned int pos)
{
    unsigned int* const ps = pows[pos];
    const bool old = exceed[pos];
    exceed[pos] = exceed[pos] || checkExceed(c, ps[0]);
    if (!exceed[pos])
        ps[0] = quickPow(c, ps[0]);
    else
    {
        if (!old)
        {
            const unsigned int p = ps[0];
            for (unsigned int i = 0; i < pcur; ++i)
                ps[i] = memp[i].pow(p >= phiMod[i + 1] ? p % phiMod[i + 1] + phiMod[i + 1] : p);
        }
        else
        {
            for (unsigned int i = 0; i < pcur; ++i)
                ps[i] = memp[i].pow(ps[i + 1] + phiMod[i + 1]);
        }
    }
}

void SegmentTree::build(const unsigned int l, const unsigned int r, SegmentTree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1)
    {
        pows[l][0] = a[l];
        val = a[l];
        unchanged = false;
        return;
    }
    lc = ++cur;
    lc->build(l, mv, cur);
    rc = ++cur;
    rc->build(mv, r, cur);
    pushup();
}
void SegmentTree::pow(const unsigned int l, const unsigned int r)
{
    if (unchanged)
        return;
    if (rv - lv == 1)
    {
        powC(lv);
        ++counter[lv];
        unchanged = counter[lv] >= maxu;
        val = pows[lv][0] % mod;
        return;
    }
    if (r <= mv)
        lc->pow(l, r);
    else if (l >= mv)
        rc->pow(l, r);
    else
    {
        lc->pow(l, mv);
        rc->pow(mv, r);
    }
    pushup();
}
unsigned int SegmentTree::sum(const unsigned int l, const unsigned int r) const
{
    if (l == lv && r == rv)
        return val;
    if (r <= mv)
        return lc->sum(l, r);
    else if (l >= mv)
        return rc->sum(l, r);
    else
        return (lc->sum(l, mv) + rc->sum(mv, r)) % mod;
}
inline void SegmentTree::pushup()
{
    this->val = (lc->val + rc->val) % mod;
    this->unchanged = lc->unchanged && rc->unchanged;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m >> mod >> c;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, l, r;
        cin >> op >> l >> r;
        if (op)
            cout << root->sum(l, r + 1) << "\n";
        else
            root->pow(l, r + 1);
    }
    return 0;
}