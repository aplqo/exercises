#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
using namespace std;
using num_t = unsigned int;
const int maxn = 1e6, maxb = 1000;

num_t dat[maxn + 10];
class block
{
public:
    block() = default;
    void init(unsigned int l, unsigned int r)
    {
        this->lv = l;
        this->rv = r;
        for (num_t* i = dat + l; i < dat + r; ++i)
        {
            auto it = ma.find(*i);
            if (it == ma.end())
                ma[*i] = 1;
            else
                ++(it->second);
        }
        ready = false;
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
            auto it = ma.find(*i);
            --(it->second);
            if (!it->second)
                ma.erase(it);
            *i += v;
            it = ma.find(*i);
            if (it == ma.end())
                ma[*i] = 1;
            else
                ++(it->second);
        }
    }
    unsigned int query(unsigned int l, unsigned int r, const num_t v)
    {
        if (v < off)
            return r - l;
        const num_t d = v - off;
        if (l == lv && r == rv)
        {
            if (!ready)
            {
                num = 0;
                for (auto i : ma)
                {
                    seq[num + 1] = i.first;
                    result[num + 1] = i.second;
                    ++num;
                }
                partial_sum(result + 1, result + 1 + num, result + 1);
                ready = true;
            }
            unsigned int pos = lower_bound(seq + 1, seq + 1 + num, d) - seq;
            return pos == num + 1 ? 0 : result[num] - result[pos - 1];
        }
        return count_if(dat + l, dat + r, [&d](const num_t i) { return i >= d; });
    }

private:
    num_t off = 0;
    map<num_t, unsigned int> ma;
    bool ready = false;
    num_t seq[maxb + 10];
    unsigned int result[maxb + 10], num = 0;
    unsigned int lv, rv;
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