#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
const unsigned int maxk = 501;
constexpr unsigned long long mod = 998244353;
constexpr unsigned long long inv2 = 499122177, inv6 = 166374059;

struct Number
{
#define decl(x)                                                                                                            \
    inline constexpr friend Number operator x(const Number l, const Number r) { return Number { (l.val x r.val) % mod }; } \
    inline constexpr void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline constexpr friend Number operator-(const Number l, const Number r)
    {
        return Number { (l.val + mod - r.val) % mod };
    }
    inline constexpr void operator-=(const Number r) { val = (val + mod - r.val) % mod; }
    inline friend std::ostream& operator<<(std::ostream& os, const Number v) { return os << v.val; }

    unsigned long long val;

    static const Number unit;
};
const Number Number::unit { 1 };
template <class T>
constexpr T quickPow(T val, unsigned long long e)
{
    T ret = T::unit;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= val;
        val *= val;
    }
    return ret;
}
inline constexpr Number inverse(const Number v)
{
    return quickPow(v, mod - 2);
}

template <unsigned int base>
struct Complex
{
    inline friend constexpr Complex operator*(const Complex a, const Complex b)
    {
        return Complex {
            (a.real.val * b.real.val + a.imag.val * b.imag.val * base) % mod,
            (a.real.val * b.imag.val + a.imag.val * b.real.val) % mod
        };
    }
    inline friend constexpr Complex operator*(const Complex a, const Number b)
    {
        return Complex { a.real * b, a.imag * b };
    }
#define decl(x)                                                                                                                           \
    inline friend constexpr Complex operator x(const Complex a, const Complex b) { return Complex { a.real x b.real, a.imag x b.imag }; } \
    inline void constexpr operator x##=(const Complex r)                                                                                  \
    {                                                                                                                                     \
        real x## = r.real;                                                                                                                \
        imag x## = r.imag;                                                                                                                \
    }
    decl(+);
    decl(-);
#undef decl
    inline void constexpr operator*=(const Complex b)
    {
        *this = *this * b;
    }

    Number real, imag;
    static constexpr Complex unit { 1 };
};
template <unsigned int base>
static constexpr Complex<base> inverse(const Complex<base> v)
{
    const Number inv = inverse(v.real * v.real - v.imag * v.imag * Number { base });
    return Complex<base> { v.real * inv, Number { mod - inv.val } * v.imag };
}

template <unsigned int width>
class Sum
{
public:
    typedef ::Complex<width == 2 ? 5 : 3> Complex;

    static void init(const unsigned int k)
    {
        initStirling(k);
        initC(k);
        initFrac(k);
        initPow(k, a, powa);
        initPow(k, b, powb);
        initPow(k, x1, powx1);
        initPow(k, x2, powx2);
    }
    static Number count(const unsigned long long l, const unsigned long long r, const unsigned int k)
    {
        Complex ret {};
        for (unsigned int i = 0; i <= k; ++i)
        {
            Complex acc {};
            for (unsigned int j = 0; j <= i; ++j)
                acc += powa[j] * powb[i - j] * seriesSum(powx1[j] * powx2[i - j], l, r) * c[i][j];
            ret += acc * stir[k][i] * ((k - i) & 0x01 ? Number { mod - 1 } : Number { 1 });
        }
        return ret.real * inverse(frac[k]);
    }

    static constexpr Complex x1 = width == 2 ? Complex { inv2, inv2 } : Complex { 2, 1 },
                             x2 = width == 2 ? Complex { inv2, mod - inv2 } : Complex { 2, mod - 1 };
    static constexpr Complex a = width == 2 ? inverse(Complex { 0, 1 }) : Complex { inv2, inv6 },
                             b = width == 2 ? inverse(Complex { 0, mod - 1 }) : Complex { inv2, mod - inv6 };

private:
    static Number stir[maxk + 10][maxk + 10], c[maxk + 10][maxk + 10], frac[maxk + 10];
    static Complex powa[maxk + 10], powb[maxk + 10], powx1[maxk + 10], powx2[maxk + 10];

    static void initFrac(const unsigned int k)
    {
        frac[0].val = 1;
        for (unsigned int i = 1; i <= k; ++i)
            frac[i] = frac[i - 1] * Number { i };
    }
    static void initC(const unsigned int k)
    {
        c[0][0].val = 1;
        for (unsigned int i = 1; i <= k; ++i)
        {
            c[i][0].val = 1;
            for (unsigned int j = 1; j <= i; ++j)
                c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
        }
    }
    static void initStirling(const unsigned int k)
    {
        stir[0][0].val = 1;
        for (unsigned int i = 1; i <= k; ++i)
            for (unsigned int j = 1; j <= i; ++j)
                stir[i][j] = Number { (stir[i - 1][j - 1].val + (i - 1) * stir[i - 1][j].val) % mod };
    }
    static void initPow(const unsigned int k, const Complex p, Complex dest[])
    {
        dest[0] = Complex::unit;
        for (unsigned int i = 1; i <= k; ++i)
            dest[i] = dest[i - 1] * p;
    }
    static Complex seriesSum(const Complex p, const unsigned long long l, const unsigned long long r)
    {
        return p.real.val == 1 && p.imag.val == 0 ? p * Number { (r + 1 - l) % mod }
                                                  : quickPow(p, l) * (quickPow(p, r + 1 - l) - Complex { 1 }) * inverse(p - Complex { 1 });
    }
};
#define decl(name) template <unsigned int base> \
decltype(Sum<base>::name) Sum<base>::name
decl(stir);
decl(c);
decl(frac);
#undef decl
#define decl(name) template <unsigned int base> \
typename Sum<base>::Complex Sum<base>::name[maxk + 10]
decl(powa);
decl(powb);
decl(powx1);
decl(powx2);
#undef decl

template <unsigned int base>
void solve(const unsigned int t)
{
    Sum<base>::init(maxk);
    for (unsigned int i = t; i; --i)
    {
        unsigned long long l, r;
        unsigned int k;
        std::cin >> l >> r >> k;
        const unsigned long long length = r + 1 - l;
        if constexpr (base == 3)
        {
            r >>= 1;
            l = (l + 1) >> 1;
        }
        else
        {
            ++l;
            ++r;
        }
        std::cout << Sum<base>::count(l, r, k) * inverse(Number { length % mod }) << "\n";
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int t, m;
    std::cin >> t >> m;
    if (m == 2)
        solve<2>(t);
    else
        solve<3>(t);
    return 0;
}