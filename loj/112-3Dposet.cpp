#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5;

struct Object
{
    unsigned int a, b, c;
    unsigned int ans = 0, cnt = 0;

    inline bool operator<(const Object& r) const
    {
        if (a != r.a)
            return a < r.a;
        else if (b != r.b)
            return b < r.b;
        return c < r.c;
    }
    inline bool operator!=(const Object& r) const
    {
        return a != r.a || b != r.b || c != r.c;
    }
} obj[maxn + 1], tmp[maxn + 1];
unsigned int cnt[maxn + 1];

namespace ftree
{
#define lowbit(x) ((x) & -(x))
    unsigned int val[maxn + 1], stk[maxn + 1], *top = stk;
    void increase(unsigned int p, const unsigned int v)
    {
        static const auto inc = [](const unsigned int p) -> unsigned int& {
            if (!val[p])
                *(top++) = p;
            return val[p];
        };
        for (; p <= maxn; p += lowbit(p))
            inc(p) += v;
    }
    void clear()
    {
        for (unsigned int* i = stk; i < top; ++i)
            val[*i] = 0;
        top = stk;
    }
    unsigned int query(unsigned int p)
    {
        unsigned int ret = 0;
        for (; p; p -= lowbit(p))
            ret += val[p];
        return ret;
    }

#undef lowbit
}

static void discretize(const unsigned int n)
{
    static unsigned int orig[maxn + 1];
    transform(obj, obj + n, orig, [](const Object& a) { return a.c; });
    sort(orig, orig + n);
    const unsigned int* const e = unique(orig, orig + n);
    for (Object* i = obj; i < obj + n; ++i)
        i->c = lower_bound<const unsigned int*>(orig, e, i->c) - orig + 1;
}
void conquer(const unsigned int l, const unsigned int r)
{
    if (r - l == 1)
        return;
    const unsigned int mid = (l + r + 1) >> 1;
    conquer(l, mid);
    conquer(mid, r);
    copy(obj + l, obj + r, tmp + l);
    Object *pl = tmp + l, *pr = tmp + mid;
    Object *const le = tmp + mid, *const re = tmp + r;
    for (unsigned int i = l; i < r; ++i)
    {
        if (pr == re)
            obj[i] = *(pl++);
        else if (pl < le && pl->b <= pr->b)
        {
            obj[i] = *pl;
            ftree::increase(pl->c, pl->cnt);
            ++pl;
        }
        else
        {
            pr->ans += ftree::query(pr->c);
            obj[i] = *(pr++);
        }
    }
    ftree::clear();
}
unsigned int unique(const unsigned int n)
{
    Object* p = obj;
    p->cnt = 1;
    for (Object* i = obj + 1; i < obj + n; ++i)
        if (*i != *(i - 1))
        {
            *(++p) = *i;
            p->cnt = 1;
        }
        else
            ++(p->cnt);
    return p + 1 - obj;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (Object* i = obj; i < obj + n; ++i)
        cin >> i->a >> i->b >> i->c;
    discretize(n);
    sort(obj, obj + n);
    conquer(0, unique(n));
    for (Object* i = obj; i < obj + n; ++i)
        if (i->cnt)
            cnt[i->ans + (i->cnt - 1)] += i->cnt;
    copy(cnt, cnt + n, ostream_iterator<unsigned int>(cout, "\n"));
    return 0;
}