#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxl = 12, maxn = 1u << maxl;
constexpr unsigned long long mod = 998244353, primitiveRoot = 3;

class Number
{
public:
#define decl(x)                                                                                                  \
    inline friend Number operator x(const Number l, const Number r) { return Number { (l.val x r.val) % mod }; } \
    inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline friend Number operator-(const Number l, const Number r)
    {
        return Number { l.val < r.val ? l.val + mod - r.val : l.val - r.val };
    }
    inline void operator-=(const Number r)
    {
        val = val >= r.val ? val - r.val : val + mod - r.val;
    }
    inline Number operator-() const { return Number { val ? mod - val : 0 }; }
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }

    unsigned long long val;
};
template <class T>
struct Pointer
{
    T* ptr;
    unsigned int size;
};
typedef Pointer<const Number> ConstArrayPtr;
typedef Pointer<Number> ArrayPtr;

Number unit[2][maxl + 10], fac[maxn + 10], ifac[maxn + 10], inv[maxn + 10];
unsigned int rev[maxl + 10][maxn + 10], lg[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
    Number ret { 1 };
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
inline Number inverse(const Number a)
{
    return quickPow(a, mod - 2);
}
static void initRev(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unsigned int* const crev = rev[i];
        for (unsigned int j = 0; j < i; ++j)
            crev[1u << j] = 1u << (i - 1 - j);
        const unsigned int n = 1u << i;
        for (unsigned int j = 3; j < n; ++j)
        {
            const unsigned int lb = j & -j;
            crev[j] = crev[j ^ lb] | crev[lb];
        }
    }
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n < (1u << i))
            return i;
}
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(Number { primitiveRoot }, (mod - 1) >> i);
        unit[1][i] = inverse(unit[0][i]);
    }
}
static void initFactor(const unsigned int n)
{
    fac[0].val = ifac[0].val = 1;
    for (unsigned int i = 1; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * Number { i };
        ifac[i] = ifac[i - 1] * inv[i];
    }
}
static void initModularInv(const unsigned int n)
{
    inv[0].val = inv[1].val = 1;
    for (unsigned int i = 2; i <= n; ++i)
        inv[i] = -inv[mod % i] * Number { mod / i };
}
static void initLog(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
        std::fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
}
static void init(const unsigned int n)
{
    const unsigned int lgn = Log2(n), v = 1u << (lgn + 1);
    initUnit(lgn + 1);
    initModularInv(v);
    initFactor(v);
    initRev(lgn + 1);
    initLog(lgn + 1);
}

void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int lgn)
{
    const unsigned int* const crev = rev[lgn];
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 0; i < src.size; ++i)
        dest[crev[i]] = src.ptr[i];
    for (unsigned int i = src.size; i < n; ++i)
        dest[crev[i]].val = 0;
}
template <bool typ>
void dft(const ConstArrayPtr a, Number dest[], const unsigned int lgn)
{
    reverseCopy(dest, a, lgn);
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        const Number wm = unit[typ][i];
        const unsigned int step = 1u << i, half = 1u << (i - 1);
        for (Number* blk = dest; blk < dest + n; blk += step)
        {
            Number w { 1 };
            for (Number *i = blk, *j = blk + half; i < blk + half; ++i, ++j)
            {
                const Number u = *i, v = *j * w;
                *i = u + v;
                *j = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (typ)
    {
        const Number iv = inv[n];
        for (unsigned int i = 0; i < n; ++i)
            dest[i] *= iv;
    }
}
Number product(const unsigned int x[4], const unsigned int msk, const unsigned int n)
{
    if (!n)
        return Number { 1 };
    static Number tmp[4][maxn + 10];
    const unsigned int lgn = lg[x[0] + x[1] + x[2] + x[3]] + 1, v = 1u << lgn;
    for (unsigned int i = 0; i < 4; ++i)
        dft<false>(ConstArrayPtr { ifac, std::min(x[i] - msk + 1, n + 1) }, tmp[i], lgn);
    for (unsigned int i = 0; i < v; ++i)
        tmp[0][i] *= tmp[1][i] * tmp[2][i] * tmp[3][i];
    dft<true>(ConstArrayPtr { tmp[0], v }, tmp[1], lgn);
    return tmp[1][n];
}

inline Number C(const unsigned int n, const unsigned int m)
{
    return fac[n] * ifac[m] * ifac[n - m];
}
Number count(const unsigned int n, const unsigned int x[])
{
    Number ret { 0 };
    const unsigned int mn = *std::min_element(x, x + 4);
    for (unsigned int i = 0; i * 4 <= n && i <= mn; ++i)
    {
        const Number p = C(n - 3 * i, i) * fac[n - 4 * i] * product(x, i, n - 4 * i);
        ret += i & 0x01 ? Number { mod - p.val } : p;
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, x[4];
    std::cin >> n >> x[0] >> x[1] >> x[2] >> x[3];
    init(x[0] + x[1] + x[2] + x[3]);
    std::cout << count(n, x) << "\n";
    return 0;
}