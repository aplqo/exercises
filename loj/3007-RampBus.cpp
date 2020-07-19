#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 500, maxt = 4e6;

int f[maxt + 10 + maxn], t[maxt + 10], p[maxt + 10];
unsigned int q[maxt + 10 + maxn], *front = q, *back = q;
#define X(j) (p[j])
#define K(i) (i)
#define Y(j) (f[j] + t[j])
struct Vector
{
    Vector(const long long x, const long long y)
        : x(x)
        , y(y)
    {
    }
    long long x, y;
};
inline long long operator*(const Vector a, const Vector b)
{
    return a.x * b.y - a.y * b.x;
}
struct Point
{
    Point(const long long x, const long long y)
        : x(x)
        , y(y)
    {
    }
    Point(const unsigned int i)
        : x(X(i))
        , y(Y(i))
    {
    }
    long long x, y;
};
inline Vector operator-(const Point a, const Point b)
{
    return Vector(a.x - b.x, a.y - b.y);
}

inline unsigned int getFront(const int i)
{
    while (back - front >= 2 && (Point(*(front + 1)) - Point(*front)) * Vector(1, K(i)) >= 0)
        ++front;
    return *front;
}
inline void push(const unsigned int i)
{
    const Point cp(i);
    while (back - front >= 2 && (cp - Point(*(back - 1))) * (cp - Point(*(back - 2))) >= 0)
        --back;
    *(back++) = i;
}

void solve(const unsigned int m, const unsigned int mt)
{
    for (unsigned int i = 1; i < m; ++i)
        f[i] = i * p[i] - t[i];
    for (unsigned int i = m; i <= mt + m; ++i)
    {
        push(i - m);
        const unsigned int lst = getFront(i);
        f[i] = f[lst] + i * (p[i] - p[lst]) - (t[i] - t[lst]);
    }
}
int main()
{
    freopen("bus.in", "r", stdin);
    freopen("bus.out", "w", stdout);
    unsigned int n, m;
    unsigned int mt = 0;
    cin >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int ti;
        cin >> ti;
        mt = max(mt, ti);
        ++p[ti];
        t[ti] += ti;
    }
    partial_sum(t, t + mt + m + 2, t);
    partial_sum(p, p + mt + m + 2, p);
    solve(m, mt);
    cout << *min_element(f + mt, f + mt + m + 1) << endl;
    return 0;
}
