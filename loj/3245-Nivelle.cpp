#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;
const unsigned int maxn = 100000, maxc = 26;
typedef float real;
constexpr real inf = numeric_limits<real>::max();

struct Range
{
    unsigned int l, r;
    inline unsigned int length() const { return r + 1 - l; }
};
struct Toy
{
    unsigned int color;
    Toy *prev, *next;
} a[maxn + 10];

static unsigned int initNext(const unsigned int n)
{
    Toy* lst[maxn + 10] {};
    for (Toy* i = a + 1; i < a + 1 + n; ++i)
    {
        i->prev = lst[i->color];
        if (lst[i->color])
            lst[i->color]->next = i;
        lst[i->color] = i;
    }
    for (unsigned int i = 0; i < maxc; ++i)
        if (lst[i])
            lst[i]->next = a + n + 1;
    return maxc - count(lst, lst + maxc, nullptr);
}
const Toy* colorKPrefix(const unsigned int n, const unsigned int k)
{
    unsigned int cnt = 0;
    for (const Toy* i = a + 1; i < a + 1 + n; ++i)
    {
        cnt += !(i->prev);
        if (cnt == k)
            return i;
    }
    return a + n + 1;
}
pair<Range, real> findLongest(const unsigned int n, const unsigned k)
{
    Range ret { 1, 0 };
    const Toy* pl = a + 1;
    unsigned int cnt = k - 1;
    for (const Toy* i = colorKPrefix(n, k); i < a + n + 1; ++i)
    {
        cnt += i->prev < pl;
        while (cnt > k)
        {
            cnt -= pl->next > i;
            ++pl;
        }
        if (i + 1 - pl > ret.length())
            ret = Range { pl - a, i - a };
    }
    return make_pair(ret, k / static_cast<real>(ret.length()));
}
Range findMinColorful(const unsigned int n, const unsigned int colors)
{
    real ans = inf;
    Range ret { 1, 0 };
    for (unsigned int i = 1; i <= colors; ++i)
    {
        const auto [rg, va] = findLongest(n, i);
        if (va < ans)
        {
            ans = va;
            ret = rg;
        }
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (Toy* i = a + 1; i < a + 1 + n; ++i)
    {
        char c;
        cin >> c;
        i->color = c - 'a';
    }
    const Range r = findMinColorful(n, initNext(n));
    cout << r.l << " " << r.r << "\n";
    return 0;
}