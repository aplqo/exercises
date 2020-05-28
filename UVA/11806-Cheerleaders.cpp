#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 20, maxk = 500;
constexpr unsigned int mod = 1e6 + 7;

class Number
{
public:
    Number() = default;
    Number(unsigned int i)
        : val(i % mod)
    {
    }
#define decl(x)                                                      \
    inline friend Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(*);
    decl(+);
    decl(-);
#undef decl
    inline explicit operator unsigned int()
    {
        return val;
    }

private:
    unsigned int val;
};
inline Number operator+(const Number l, const Number r) { return l.val + r.val; }
inline Number operator-(const Number l, const Number r) { return l.val + mod - r.val; }
inline Number operator*(const Number l, const Number r) { return (1ull * l.val * r.val) % mod; }
Number c[maxn * maxn + 1][maxn * maxn + 1];

void preC(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        c[i][0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
}
unsigned int solve(const unsigned int n, const unsigned int m, const unsigned int k)
{
    const auto sn = [&k](const unsigned int n, const unsigned int m) { return c[n * m][k]; };
    return static_cast<unsigned int>(
        sn(n, m)
        - (2 * sn(n - 1, m) + 2 * sn(n, m - 1))
        + (4 * sn(n - 1, m - 1) + sn(n, m - 2) + sn(n - 2, m))
        - (2 * sn(n - 1, m - 2) + 2 * sn(n - 2, m - 1))
        + sn(n - 2, m - 2));
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    preC(maxn * maxn);
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned int n, m, k;
        cin >> n >> m >> k;
        cout << "Case " << i + 1 << ": " << (n * m >= k && k >= 2 ? solve(n, m, k) : 0) << endl;
    }
    return 0;
}