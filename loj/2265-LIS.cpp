#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
const unsigned int maxn = 5e4, maxq = 2e5;

unsigned int sqrtN;
class FTree
{
public:
    unsigned int n;

#define lowbit(x) ((x) & -(x))
    void increase(unsigned int pos)
    {
        for (; pos <= n; pos += lowbit(pos))
            ++val[pos];
    }
    unsigned int prefixSum(unsigned int pos) const
    {
        unsigned int ret = 0;
        for (; pos; pos -= lowbit(pos))
            ret += val[pos];
        return ret;
    }
    unsigned int sum(const unsigned int l, const unsigned int r) const
    {
        return prefixSum(r) - prefixSum(l - 1);
    }
#undef lowbit

private:
    unsigned int val[maxn + 10];
};
template <template <typename> class Pred, bool column>
class YoungTableau
{
public:
    void init(const unsigned int n)
    {
        size.n = n;
        for (unsigned int i = 0; i <= sqrtN; ++i)
            val[i].reserve(n);
    }
    void add(unsigned int v)
    {
        for (unsigned int i = 1; i <= sqrtN; ++i)
        {
            auto p = std::upper_bound(val[i].begin(), val[i].end(), v, pred);
            if (p == val[i].end())
            {
                val[i].push_back(v);
                if constexpr (column)
                    size.increase(val[i].size());
                else
                    size.increase(i);
                return;
            }
            std::swap(v, *p);
        }
    }
    unsigned int count(const unsigned int l, const unsigned int r) const
    {
        return size.sum(l, r);
    }

private:
    std::vector<unsigned int> val[maxn + 10];
    FTree size;
    static Pred<unsigned int> pred;
};
struct Query
{
    unsigned int m, k, ans;
} qry[maxq + 10];
unsigned int b[maxn + 10];
YoungTableau<std::greater, false> orig;
YoungTableau<std::less_equal, true> trans;

static void init(const unsigned int n)
{
    sqrtN = std::sqrt(n);
    orig.init(n);
    trans.init(n);
}
void add(const unsigned int x)
{
    orig.add(x);
    trans.add(x);
}
unsigned int count(const unsigned int k)
{
    if (k <= sqrtN)
        return orig.count(1, k);
    else
        return orig.count(1, sqrtN) + trans.count(sqrtN + 1, k);
}
void answerQuery(const unsigned int q)
{
    static Query* seq[maxq + 10];
    std::iota(seq, seq + q, qry);
    std::sort(seq, seq + q, [](const Query* a, const Query* b) { return a->m < b->m; });
    unsigned int cur = 1;
    for (unsigned int i = 0; i < q; ++i)
    {
        while (cur <= seq[i]->m)
            add(b[cur++]);
        seq[i]->ans = count(seq[i]->k);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, q;
    std::cin >> n >> q;
    std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, b + 1);
    for (Query* i = qry; i < qry + q; ++i)
        std::cin >> i->m >> i->k;
    init(n);
    answerQuery(q);
    for (const Query* i = qry; i < qry + q; ++i)
        std::cout << i->ans << "\n";
    return 0;
}