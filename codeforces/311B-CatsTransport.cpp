#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5, maxp = 100;

long long d[maxn + 1], t[maxm + 1], sum[maxm + 1], f[maxp + 1][maxm + 1];

#define X(j) (j)
#define Y(j, k) (f[k][j] + sum[j])
#define K(i) t[i]
struct Vector
{
    inline long long operator*(const Vector r) const { return x * r.y - y * r.x; }
    long long x, y;
};
struct Point
{
    Point(const unsigned int i, const unsigned int k)
        : x(X(i))
        , y(Y(i, k)) {};
    long long x, y;
    inline Vector operator-(const Point r) const { return Vector { x - r.x, y - r.y }; }
};
class Convex
{
public:
    unsigned int pop(const unsigned int i);
    void push(const unsigned int i);
    unsigned int k;

private:
    unsigned int q[maxm + 10] = {}, *front = q, *back = q;
};

unsigned int Convex::pop(const unsigned int i)
{
    const Vector cv { 1, K(i) };
    while (back - front > 1 && cv * (Point(*(front), k) - Point(*(front + 1), k)) >= 0)
        ++front;
    return *front;
}
void Convex::push(const unsigned int i)
{
    const Point cp(i, k);
    while (back - front > 1 && (cp - Point(*(back - 1), k)) * (cp - Point(*(back - 2), k)) >= 0)
        --back;
    *(back++) = i;
}

long long dp(const unsigned int m, const unsigned int p)
{
    static Convex cv[maxp + 10];
    for (unsigned int i = 0; i < m; ++i)
        f[1][i] = (i + 1) * t[i] - sum[i];
    for (unsigned int j = 2; j <= p; ++j)
    {
        cv[j - 1].k = j - 1;
        for (unsigned int i = 0; i < m; ++i)
        {
            cv[j - 1].push(i);
            const unsigned int fr = cv[j - 1].pop(i);
            f[j][i] = f[j - 1][fr] + (i - fr) * t[i] - (sum[i] - sum[fr]);
        }
    }
    return f[p][m - 1];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, p;
    cin >> n >> m >> p;
    copy_n(istream_iterator<long long>(cin), n - 1, d + 2);
    partial_sum(d, d + 1 + n, d);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int h;
        cin >> h >> t[i];
        t[i] -= d[h];
    }
    sort(t, t + m);
    partial_sum(t, t + m, sum);
    cout << dp(m, p) << "\n";
    return 0;
}