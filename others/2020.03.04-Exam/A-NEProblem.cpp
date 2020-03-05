#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
constexpr unsigned int inf = UINT_MAX;
const int maxq = 44723;

enum
{
    OPER = 0,
    RES = 1
};
enum
{
    LEFT = 0,
    RIGHT = 1
};
struct factor
{
    num_t val;
    unsigned int ans[2] = { 0, inf };
    unsigned int tmp[2];
    bool fixed = false;
} res[maxq + 10], *pcur = res, *cur = res;
unsigned int mindiv[maxq + 10];

inline void euler(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            pcur->val = i;
            ++pcur;
        }
        for (factor* j = res; j < pcur && j->val <= mindiv[i] && j->val * i <= n; ++j)
            mindiv[j->val * i] = j->val;
    }
}
template <unsigned int pos>
inline void fact(num_t v)
{
    for_each(res, pcur, [](factor& i) { i.tmp[pos] = 0; });
    for (factor* i = res; i < pcur && v > 1; ++i)
        if (!(v % i->val))
        {
            for (; !(v % i->val); v /= i->val)
                ++(i->tmp[pos]);
        }
    if (v > 1)
    {
        factor* p = find_if(pcur, cur, [&v](const factor& i) { return i.val == v; });
        if (cur == p)
        {
            p->val = v;
            p->tmp[0] = p->tmp[1] = 0;
            ++cur;
        }
        p->tmp[pos] = 1;
    }
}
template <class T, unsigned int typ>
inline bool calc()
{
    T cmp;
    for (factor* i = res; i < cur; ++i)
    {
        if (i->tmp[OPER] == i->tmp[RES])
            i->ans[typ] = cmp(i->ans[typ], i->tmp[RES]) ? i->ans[typ] : i->tmp[RES];
        else
        {
            if (i->fixed && i->ans[LEFT] != i->tmp[RES])
                return false;
            i->fixed = true;
            i->ans[0] = i->ans[1] = i->tmp[RES];
        }
    }
    return true;
}
inline void solve()
{
    num_t a0, a1, b0, b1, ans = 1;
    cin >> a0 >> a1 >> b0 >> b1;
    cur = pcur;
    for_each(res, pcur, [](factor& i) {i.ans[LEFT]=0;i.ans[RIGHT]=inf;i.fixed=false; });
    fact<OPER>(a0);
    fact<RES>(a1);
    if (!calc<greater<num_t>, LEFT>())
        goto fail;
    fact<OPER>(b0);
    fact<RES>(b1);
    if (!calc<less<num_t>, RIGHT>())
        goto fail;
    for (factor* i = res; i < cur; ++i)
    {
        if (i->ans[LEFT] > i->ans[RIGHT])
            goto fail;
        ans *= (i->ans[RIGHT] + 1 - i->ans[LEFT]);
    }
    cout << ans << endl;
    return;
fail:;
    cout << "0" << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    euler(maxq);
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
        solve();
    return 0;
}
