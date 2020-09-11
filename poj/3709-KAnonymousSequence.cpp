#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 500000;
const long long inf = INT_MAX;

long long f[maxn + 1], sum[maxn + 1], a[maxn + 1];
#define X(j) a[(j) + 1]
#define Y(j) (f[j] - sum[j] + (j)*a[(j) + 1])
#define K(i) (i)
struct Vector
{
    Vector(long long x, long long y)
        : x(x)
        , y(y) {};
    long long x, y;
    inline long long operator*(const Vector& r) const { return x * r.y - y * r.x; }
};
struct Point
{
    Point(const unsigned int i)
        : x(X(i))
        , y(Y(i)) {};
    long long x, y;
    inline Vector operator-(const Point& r) const { return Vector(x - r.x, y - r.y); }
};
class Convex
{
public:
    void clear();
    void push(const unsigned int i);
    unsigned int pop(const unsigned int i);

private:
    unsigned int q[maxn + 1], *front, *back;
};
void Convex::clear()
{
    front = back = q;
}
unsigned int Convex::pop(const unsigned int i)
{
    const Vector cv(1, K(i));
    while (back - front > 1 && cv * (Point(*front) - Point(*(front + 1))) >= 0)
        ++front;
    return *front;
}
void Convex::push(const unsigned int i)
{
    const Point cp(i);
    while (back - front > 1 && (cp - Point(*(back - 1))) * (cp - Point(*(back - 2))) >= 0)
        --back;
    *(back++) = i;
}

long long dp(const unsigned int n, const unsigned int k)
{
    static Convex cv;
    fill(f + 1, f + k, inf);
    f[k] = sum[k] - a[1] * k;
    cv.clear();
    cv.push(0);
    for (unsigned int i = k + 1; i <= n; ++i)
    {
        cv.push(i - k);
        const unsigned int fr = cv.pop(i);
        f[i] = f[fr] + (sum[i] - sum[fr]) - a[fr + 1] * (i - fr);
    }
    return f[n];
}
void solve()
{
    unsigned int n, k;
    cin >> n >> k;
    for (long long* i = a + 1; i < a + n + 1; ++i)
        cin >> *i;
    partial_sum(a + 1, a + 1 + n, sum + 1);
    cout << dp(n, k) << "\n";
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}