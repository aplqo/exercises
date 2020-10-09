#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <deque>
#include <iostream>
#include <numeric>
#include <string>
using namespace std;
typedef long double cost_t;
const char delim[] = "--------------------";
const unsigned int maxn = 1e5;

struct Select
{
    unsigned int pos;
    unsigned int l, r;
};
deque<Select> q;
string s[maxn + 10];
int sum[maxn + 10], l;
unsigned int p, n;
cost_t f[maxn + 10];
unsigned int from[maxn + 10];

inline cost_t quickPow(cost_t a, unsigned int e)
{
    cost_t ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
inline cost_t getCost(const int pl, const int pr)
{
    return quickPow(abs(sum[pr] - sum[pl] + pr - pl - 1 - l), p) + f[pl];
}
unsigned int binarySearch(const Select last, const unsigned int npos)
{
    unsigned int l = last.l, r = last.r;
    while (l <= r)
    {
        const unsigned int mid = (l + r) >> 1;
        if (getCost(npos, mid) > getCost(last.pos, mid))
            l = mid + 1;
        else
            r = mid - 1;
    }
    return l;
}
void pushCand(const unsigned int pos)
{
    while (!q.empty())
    {
        const Select s = q.back();
        if (getCost(pos, s.l) < getCost(s.pos, s.l))
            q.pop_back();
        else
            break;
    }
    if (q.empty())
        q.push_back(Select { pos, 1, n });
    else
    {
        const unsigned int pl = binarySearch(q.back(), pos);
        if (pl <= n)
        {
            q.back().r = pl - 1;
            q.push_back(Select { pos, pl, n });
        }
    }
}
void dp(const unsigned int n)
{
    q.push_back(Select { 0, 1, n });
    for (unsigned int i = 1; i <= n; ++i)
    {
        while (q.front().r < i)
            q.pop_front();
        const Select t = q.front();
        from[i] = t.pos;
        f[i] = getCost(t.pos, i);
        pushCand(i);
    }
}
void printSolution(const unsigned int p)
{
    if (!p)
        return;
    printSolution(from[p]);
    for (unsigned int i = from[p] + 1; i < p; ++i)
        cout << s[i] << " ";
    cout << s[p] << "\n";
}
void solve()
{
    cin >> n >> l >> p;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cin >> s[i];
        sum[i] = s[i].size();
    }
    partial_sum(sum, sum + 1 + n, sum);
    dp(n);
    if (f[n] > 1e18)
        cout << "Too hard to arrange\n";
    else
    {
        cout << static_cast<unsigned long long>(f[n]) << "\n";
        printSolution(n);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        cout << delim << "\n";
        q.clear();
    }
    return 0;
}