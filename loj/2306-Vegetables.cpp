#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include <cassert>

const unsigned int maxn = 1e5, maxm = 1e5, maxp = 1e5;

struct Vegetable
{
    unsigned int price, extra, count, decrease;
    unsigned int soldCount, currentPrice, available, lastUse;

    template <bool first>
    unsigned long long sell(const unsigned long long cnt)
    {
        const unsigned long long ret = currentPrice * cnt;
        soldCount += cnt;
        available -= cnt;
        if constexpr (first)
            currentPrice = price;
        return ret;
    }
    void updateAvailable(const unsigned int day)
    {
        available += (lastUse - day) * decrease;
        lastUse = day;
    }
} veg[maxn + 10];
std::vector<Vegetable*> newVegetable[maxp + 10];
unsigned int query[maxp + 10];
unsigned long long ans[maxp + 10];

static void init(const unsigned int n, const unsigned int maxP)
{
    for (Vegetable* i = veg; i < veg + n; ++i)
    {
        unsigned int last;
        if (i->decrease)
        {
            last = std::min((i->count + i->decrease - 1) / i->decrease, maxP);
            i->available = i->count - (last - 1) * i->decrease;
        }
        else
        {
            i->available = i->count;
            last = maxP;
        }
        newVegetable[last].push_back(i);
        i->lastUse = last;
    }
}
unsigned long long greedyFromLast(const unsigned int m, const unsigned int maxP)
{
    struct NodeLess
    {
        inline bool operator()(const Vegetable* l, const Vegetable* r) const
        {
            return l->currentPrice < r->currentPrice;
        }
    };
    std::priority_queue<Vegetable*, std::vector<Vegetable*>, NodeLess> q;
    unsigned long long ret = 0;
    for (unsigned int i = maxP; i; --i)
    {
        static Vegetable* stk[maxn + 10];
        for (auto j : newVegetable[i])
            q.push(j);
        Vegetable** top = stk;
        unsigned int count = m;
        while (count && !q.empty())
        {
            Vegetable* const veg = q.top();
            q.pop();
            veg->updateAvailable(i);
            if (!veg->soldCount)
            {
                ret += veg->sell<true>(1);
                --count;
                if (veg->available && count)
                    q.push(veg);
                else
                    *(top++) = veg;
            }
            else
            {
                const unsigned int cur = std::min(count, veg->available);
                ret += veg->sell<false>(cur);
                *(top++) = veg;
                count -= cur;
            }
        }
        for (Vegetable** i = stk; i < top; ++i)
            if ((*i)->count > (*i)->soldCount)
                q.push(*i);
    }
    return ret;
}
void greedyFixAns(const unsigned int n, const unsigned int m, const unsigned int maxP, unsigned long long ans)
{
    struct NodeLess
    {
        inline bool operator()(const Vegetable* a, const Vegetable* b) const
        {
            return a->currentPrice > b->currentPrice;
        }
    };
    std::priority_queue<Vegetable*, std::vector<Vegetable*>, NodeLess> q;
    unsigned int soldSum = 0;
    for (Vegetable* i = veg; i < veg + n; ++i)
        switch (i->soldCount)
        {
        case 0:
            break;
        case 1:
            i->currentPrice = i->price + i->extra;
            [[fallthrough]];
        default:
            soldSum += i->soldCount;
            q.push(i);
            break;
        }
    for (unsigned int i = maxP, maxSold = maxP * m; i; --i, maxSold -= m)
    {
        while (soldSum > maxSold)
        {
            Vegetable* const cur = q.top();
            if (cur->soldCount == 1)
            {
                q.pop();
                --soldSum;
                ans -= cur->currentPrice;
            }
            else
            {
                const unsigned long long tp = std::min({ soldSum - maxSold, cur->soldCount - 1 });
                ans -= tp * cur->currentPrice;
                soldSum -= tp;
                cur->soldCount -= tp;
                if (cur->soldCount == 1)
                {
                    q.pop();
                    cur->currentPrice = cur->price + cur->extra;
                    q.push(cur);
                }
            }
        }
        ::ans[i] = ans;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    for (Vegetable* i = veg; i < veg + n; ++i)
    {
        std::cin >> i->price >> i->extra >> i->count >> i->decrease;
        i->currentPrice = i->price + i->extra;
    }
    unsigned int maxP = 0;
    for (unsigned int* i = query; i < query + k; ++i)
    {
        std::cin >> *i;
        maxP = std::max(maxP, *i);
    }
    init(n, maxP);
    greedyFixAns(n, m, maxP, greedyFromLast(m, maxP));
    for (const unsigned int* i = query; i < query + k; ++i)
        std::cout << ans[*i] << "\n";
    return 0;
}