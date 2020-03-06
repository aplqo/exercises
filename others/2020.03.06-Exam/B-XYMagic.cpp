#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
using num_t = unsigned int;
const int maxn = 1e6, maxb = 1000, maxc = 3e6;

class stree
{
public:
    stree(const num_t l, const num_t r)
        : lv(l)
        , rv(r)
        , mv((l + r) >> 1) {};
    void modify(const num_t pos, const bool d)
    {
        static auto do_modify = [&d, &pos](stree*(&ptr), num_t lv, num_t rv) {
            if (!ptr)
                ptr = new stree(lv, rv);
            ptr->modify(pos, d);
            if (!ptr->val)
            {
                delete ptr;
                ptr = nullptr;
            }
        };
        if (rv - lv == 1)
        {
            if (d)
                ++val;
            else
                --val;
            return;
        }
        if (pos < mv)
            do_modify(lc, lv, mv);
        else
            do_modify(rc, mv, rv);
        val = GetVal(lc, lv, mv) + GetVal(rc, mv, rv);
    }
    unsigned int query(const num_t l, const num_t r) const
    {
        if (rv - lv == 1 || (l == lv && r == rv))
            return val;
        if (r <= mv)
            return GetVal(lc, l, r);
        else if (l >= mv)
            return GetVal(rc, l, r);
        else
            return GetVal(lc, l, r) + GetVal(rc, l, r);
    }

private:
    static inline unsigned int GetVal(const stree* ptr, const num_t l, const num_t r)
    {
        return ptr ? ptr->query(l, r) : 0;
    }
    const num_t lv, rv, mv;
    stree *lc = nullptr, *rc = nullptr;
    unsigned int val = 0;
};
num_t dat[maxn + 10];
class block
{
public:
    block() = default;
    void init(unsigned int l, unsigned int r)
    {
        this->lv = l;
        this->rv = r;
        for (const num_t* i = dat + l; i < dat + r; ++i)
            val.modify(*i, true);
    }
    void modify(unsigned int l, unsigned int r, const num_t v)
    {
        if (this->lv == l && this->rv == r)
        {
            off += v;
            return;
        }
        for (num_t* i = dat + l; i < dat + r; ++i)
        {
            val.modify(*i, false);
            *i += v;
            val.modify(*i, true);
        }
    }
    unsigned int query(unsigned int l, unsigned int r, const num_t v)
    {
        if (v < off)
            return r - l;
        if (v > maxc)
            return 0;
        const num_t d = v - off;
        if (l == lv && r == rv)
            return val.query(d, maxc);
        return count_if(dat + lv, dat + rv, [&d](const num_t i) { return i >= d; });
    }

private:
    num_t off = 0;
    unsigned int lv, rv;
    stree val = stree(0, maxc);
} blk[maxb + 10];
unsigned int bsize;

inline void init(const unsigned int n)
{
    bsize = sqrt(n);
    for (unsigned int i = 0; i < n / bsize; ++i)
        blk[i].init(i * bsize, (i + 1) * bsize);
    blk[n / bsize].init((n / bsize) * bsize, n);
}
inline void modify(const unsigned int l, const unsigned int r, const num_t v)
{
    const unsigned int inl = l / bsize, inr = r / bsize;
    for (unsigned int i = inl + 1; i < inr; ++i)
        blk[i].modify(i * bsize, (i + 1) * bsize, v);
    if (inl != inr)
    {
        blk[inl].modify(l, (inl + 1) * bsize, v);
        blk[inr].modify(inr * bsize, r, v);
    }
    else
        blk[inl].modify(l, r, v);
}
inline unsigned int query(const unsigned int l, const unsigned int r, num_t v)
{
    const unsigned int inl = l / bsize, inr = r / bsize;
    unsigned int ret = 0;
    for (unsigned int i = inl + 1; i < inr; ++i)
        ret += blk[i].query(i * bsize, i * bsize + bsize, v);
    if (inl != inr)
        ret += blk[inl].query(l, inl * bsize + bsize, v) + blk[inr].query(inr * bsize, r, v);
    else
        ret += blk[inl].query(l, r, v);
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (num_t* i = dat; i < dat + n; ++i)
        cin >> *i;
    init(n);
    for (unsigned int i = 0; i < q; ++i)
    {
        char op;
        unsigned int l, r;
        num_t v;
        cin >> op >> l >> r >> v;
        if (op == 'A')
            cout << query(l - 1, r, v) << endl;
        else
            modify(l - 1, r, v);
    }
    return 0;
}