#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned int inf = UINT_MAX;

struct Stick
{
    unsigned int len, col;

    inline bool operator<(const Stick& r) const { return len < r.len; }
} st[maxn + 10];
constexpr Stick sinf { 0, inf };

inline bool check(const Stick a[])
{
    return all_of(a, a + 3, [](const Stick& i) { return i.len; })
        && a[0].len + a[1].len > a[2].len;
}
inline void insert(Stick a[], const Stick v)
{
    for (Stick* i = a; i < a + 3; ++i)
        if (i->col == v.col)
        {
            *i = v;
            sort(a, a + 3);
            return;
        }
    a[0] = v;
    sort(a, a + 3);
}
inline bool find(const unsigned int n)
{
    Stick mx[4] { sinf, sinf, sinf };
    sort(st, st + n);
    for (const Stick* i = st; i < st + n; ++i)
    {
        insert(mx, *i);
        if (check(mx))
        {
            for (const Stick* i = mx; i < mx + 3; ++i)
                cout << i->col + 1 << " " << i->len << " ";
            cout.put('\n');
            return true;
        }
    }
    return false;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int k, sn = 0;
    cin >> k;
    Stick* s = st;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int n;
        cin >> n;
        sn += n;
        for (; n; --n)
        {
            cin >> s->len;
            (s++)->col = i;
        }
    }
    if (!find(sn))
        cout << "NIE\n";
    return 0;
}