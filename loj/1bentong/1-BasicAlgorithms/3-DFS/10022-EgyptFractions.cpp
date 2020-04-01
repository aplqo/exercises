#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <type_traits>
using num_t = unsigned long long;
using namespace std;
const int maxn = 1e7, maxf = 100;

class frac
{
public:
    frac() = default;
    explicit frac(num_t den)
        : num(1)
        , den(den)
    {
    }
    frac(num_t num, num_t den)
        : num(num)
        , den(den)
    {
        Reduce();
    }
    inline frac operator+(const frac r) const
    {
        return frac(num * r.den + r.num * den, r.den * den);
    }
    inline frac operator-(const frac r) const
    {
        return frac(num * r.den - r.num * den, den * r.den);
    }
    inline frac operator*(const frac r) const
    {
        return frac(num * r.num, den * r.den);
    }
#define op(o)                                  \
    inline bool operator o(const frac r) const \
    {                                          \
        return Compare(r) o 0;                 \
    }
    op(>);
    op(<);
    op(!=);
#undef op
    inline bool isUnit() const
    {
        return num == 1;
    }
    inline unsigned int MinUnit() const
    {
        return den / num + 1;
    }

    num_t num, den;

private:
    inline int Compare(const frac r) const
    {
        num_t n1 = num * r.den, n2 = r.num * den;
        if (n1 > n2)
            return 1;
        else if (n1 < n2)
            return -1;
        return 0;
    }
    inline void Reduce()
    {
        num_t g = gcd(num, den);
        num /= g;
        den /= g;
    }
};

frac ans[maxn + 10], tmp[maxn + 10];
unsigned int num = maxn + 10, limd = 1;

inline bool test(const unsigned int len)
{
    if (num != len)
        return len < num;
    for (unsigned int i = len; i > 0; --i)
        if (ans[i] != tmp[i])
            return ans[i] < tmp[i];
    return false;
}
inline unsigned int fun(const frac val, const unsigned int b)
{
    unsigned int r = (1.0 * val.num / val.den) / (1.0 / b);
    return r > 1 ? r : 0;
}

bool dfs(const frac val, const unsigned int b, const unsigned int dep)
{
    if (val.isUnit() && val.den >= b)
    {
        tmp[dep] = val;
        if (test(dep))
        {
            copy(tmp + 1, tmp + dep + 1, ans + 1);
            num = dep;
        }
        return true;
    }
    if (fun(val, b) + dep > limd || dep > num)
        return false;
    bool ret = false;
    for (unsigned int i = max(val.MinUnit(), b); i <= maxn; ++i)
    {
        const frac f(i);
        if (f * frac(min(num, limd - dep + 1), 1) < val)
            break;
        tmp[dep] = f;
        ret |= dfs(val - f, i + 1, dep + 1);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int a, b;
    cin >> a >> b;
    const frac f(a, b);
    while (!dfs(f, 1, 1))
        ++limd;
    for (unsigned int i = 1; i < num; ++i)
        cout << ans[i].den << " ";
    cout << ans[num].den << endl;
    return 0;
}