#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;
using real = long double;
constexpr real eps = 1e-9L;

real ca[maxn + 1], cb[maxn + 1], f[maxn + 1], A[maxn + 1], B[maxn + 1], Rate[maxn + 1];
unsigned int buf[maxn + 1];

inline void getCount(const unsigned int x)
{
    cb[x] = f[x] / (A[x] * Rate[x] + B[x]);
    ca[x] = cb[x] * Rate[x];
}
inline real profit(const unsigned int old, const unsigned int now)
{
    return A[now] * ca[old] + B[now] * cb[old];
}
namespace Convex
{
#define X(j) ca[j]
#define Y(j) cb[j]
#define K(i) (-A[i] / B[i])
    struct Vector
    {
        Vector(real x, real y)
            : x(x)
            , y(y) {};
        inline real operator*(const Vector r) const { return x * r.y - y * r.x; }

    private:
        real x, y;
    };
    struct Point
    {
        Point(real x, real y)
            : x(x)
            , y(y) {};
        Point(const unsigned int i)
            : Point(X(i), Y(i)) {};
        inline Vector operator-(const Point r) const { return Vector(x - r.x, y - r.y); }

    private:
        real x, y;
    };
    class Convex
    {
    public:
        inline void push(const unsigned int x)
        {
            const Point cp(x);
            while (back - front >= 2 && (cp - Point(*(back - 2))) * (cp - Point(*(back - 1))) >= -eps)
                --back;
            *(back++) = x;
        }
        inline real top(const unsigned int x)
        {
            const Vector cv(1, K(x));
            while (back - front >= 2 && cv * (Point(*(front + 1)) - Point(*front)) >= -eps)
                ++front;
            return profit(*front, x);
        }
        void clear() { front = back = q; }

    private:
        unsigned int q[maxn + 1], *front = q, *back = q;
    };
#undef Y
}

void cdq(const unsigned int l, const unsigned int r)
{
    static Convex::Convex cv;
    static unsigned int tmp[maxn + 1];
    if (r - l == 1)
    {
        f[buf[l]] = max(f[buf[l]], f[buf[l] - 1]);
        getCount(buf[l]);
        return;
    }
    const unsigned int mid = (l + r) >> 1;
    cdq(l, mid);
    for (unsigned int* i = buf + l; i < buf + mid; ++i)
        cv.push(*i);
    copy(buf + mid, buf + r, tmp + mid);
    sort(tmp + mid, tmp + r, [](unsigned int a, unsigned int b) { return K(a) > K(b); });
    for (unsigned int* i = tmp + mid; i < tmp + r; ++i)
        f[*i] = max(f[*i], cv.top(*i));
    cv.clear();
    cdq(mid, r);
    inplace_merge(buf + l, buf + mid, buf + r, [](unsigned int a, unsigned int b) { return X(a) < X(b); });
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, s;
    cin >> n >> s;
    f[0] = s;
    for (unsigned int i = 1; i <= n; ++i)
        cin >> A[i] >> B[i] >> Rate[i];
    iota(buf, buf + n + 1, 0);
    cdq(1, n + 1);
    ios_base::sync_with_stdio(true);
    printf("%.3Lf\n", *max_element(f + 1, f + 1 + n));
    return 0;
}