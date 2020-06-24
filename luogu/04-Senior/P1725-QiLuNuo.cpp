#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const int maxn = 200000;

class mqueue
{
public:
    void push(const int p, const int val)
    {
        while (!q.empty() && q.back().second < val)
            q.pop_back();
        q.emplace_back(p, val);
    }
    void pop(const int p)
    {
        while (!q.empty() && q.front().first < p)
            q.pop_front();
    }
    int front() const
    {
        return q.front().second;
    }

private:
    deque<pair<int, int>> q;
};

int a[maxn + 1], f[maxn + 1];

int dp(const int n, const int l, const int r)
{
    mqueue q;
    fill(f + 1, f + l, INT_MIN / 2);
    for (int i = l; i <= n; ++i)
    {
        q.push(i - l, f[i - l]);
        q.pop(i - r);
        f[i] = q.front() + a[i];
    }
    return *max_element(f + max(0, n - r + 1), f + n + 1);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    int n, l, r;
    cin >> n >> l >> r;
    copy_n(istream_iterator<int>(cin), n + 1, a);
    cout << dp(n, l, r) << endl;
    return 0;
}