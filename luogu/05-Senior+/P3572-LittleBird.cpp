#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 1000000;

class Slide
{
public:
    unsigned int front(const unsigned int cd) const;
    void push(const unsigned int pos, const unsigned int val);
    void pop(const unsigned int pos);

private:
    deque<pair<unsigned int, unsigned int>> q;
};
unsigned int d[maxn + 10];

unsigned int Slide::front(const unsigned int cd) const
{
    unsigned int ret = q.front().first - (d[q.front().second] > cd);
    if (const auto it = next(q.cbegin()); it != q.cend() && d[it->second] > cd)
        ret = min(ret, it->first - 1);
    return ret;
}
void Slide::push(const unsigned int pos, const unsigned int val)
{
    while (!q.empty() && (q.back().first > val || (q.back().first == val && d[q.back().second] <= d[pos])))
        q.pop_back();
    q.emplace_back(val, pos);
}
void Slide::pop(const unsigned int pos)
{
    while (q.front().second < pos)
        q.pop_front();
}

static unsigned int dp(const unsigned int n, const unsigned int k)
{
    static unsigned int f[maxn + 10];
    Slide mq;
    mq.push(1, 1);
    for (unsigned int i = 2; i < k + 1; ++i)
    {
        f[i] = mq.front(d[i]);
        mq.push(i, f[i] + 1);
    }
    for (unsigned int i = k + 1; i <= n; ++i)
    {
        mq.pop(i - k);
        f[i] = mq.front(d[i]);
        mq.push(i, f[i] + 1);
    }
    return f[n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, d + 1);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int k;
        cin >> k;
        cout << dp(n, k) << "\n";
    }
    return 0;
}