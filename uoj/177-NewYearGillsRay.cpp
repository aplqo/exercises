#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;
const unsigned int maxn = 5e4, maxm = 5e4;

long long b[maxm + 10];
multiset<long long> a;

bool check(const unsigned int m, const long long mnb, const long long x)
{
    multiset<long long> s({ x }), t = a;
    while (s.size() < t.size())
    {
        if (s.empty())
            return false;
        const auto et = prev(t.end()), es = prev(s.end());
        if (*es - mnb < *et)
        {
            const auto p = s.lower_bound(*et);
            if (p == s.end())
                return false;
            s.erase(p);
            t.erase(et);
        }
        else
        {
            const long long v = *es;
            s.erase(es);
            for (const long long* i = b; i < b + m; ++i)
                s.insert(v - *i);
        }
    }
    for (auto its = s.begin(), itt = t.begin(); its != s.end() && itt != t.end(); ++its, ++itt)
        if (*its < *itt)
            return false;
    return true;
}
long long binarySearch(const unsigned int m)
{
    const long long mnb = *min_element(b, b + m);
    long long l = *a.begin(), r = 1e18;
    while (l <= r)
    {
        const long long mid = (l + r) >> 1;
        if (check(m, mnb, mid))
            r = mid - 1;
        else
            l = mid + 1;
    }
    return l;
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = n; i; --i)
    {
        long long v;
        cin >> v;
        a.insert(v);
    }
    copy_n(istream_iterator<long long>(cin), m, b);
    cout << binarySearch(m) << "\n";
    return 0;
}