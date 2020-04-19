/*
* Luogu team T129620: adventure
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;
const int maxn = 100000;

long long val[maxn + 10];
double f[maxn + 10];

class myque
{
public:
    void push(const unsigned int i, const double val)
    {
        while (!q.empty() && q.back().second <= val)
            q.pop_back();
        q.emplace_back(i, val);
    }
    void pop(const unsigned int p)
    {
        while (!q.empty() && q.front().first > p)
            q.pop_front();
    }
    double front() const
    {
        return q.front().second;
    }

private:
    deque<pair<unsigned int, double>> q;
};

double dp(const unsigned int n, const unsigned int m, const double k)
{
    myque q;
    q.push(n, val[n]);
    for (unsigned int i = n - 1; i > n - m; --i)
    {
        f[i] = max<double>(q.front() * k + val[i], val[i]);
        q.push(i, f[i]);
    }
    for (unsigned int i = n - m; i; --i)
    {
        q.pop(i + m);
        f[i] = q.front() * k + val[i];
        q.push(i, f[i]);
    }
    double ret = numeric_limits<double>::lowest();
    for (unsigned int i = 1; i <= m; ++i)
        ret = max(ret, f[i]);
    return ret;
}
int main()
{
    unsigned int n, m;
    double k;
    cin >> n >> m >> k;
    for (long long* i = val + 1; i < val + 1 + n; ++i)
        cin >> *i;
    printf("%.2lf\n", dp(n, m, k));
    return 0;
}