#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 18, maxn = 1u << maxln;
constexpr unsigned long long mod = 998244353, primeRoot = 3;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    inline friend Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline void operator x##=(const Number r) { val = ((val + mod) x r.val) % mod; }
    decl(+);
    decl(-);
    decl(*);
#undef decl
    inline friend ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }
    inline friend istream& operator>>(istream& is, Number& v)
    {
        return is >> v.val;
    }
    inline Number operator-() const
    {
        Number ret;
        ret.val = val ? mod - val : 0;
        return ret;
    }

private:
    unsigned long long val = 0;
};
struct ConstArrayPtr
{
    const Number* ptr;
    unsigned int size;
};
struct ArrayPtr
{
    Number* ptr;
    unsigned int size;

    inline operator ConstArrayPtr() const { return ConstArrayPtr { ptr, size }; }
};
Number unit[2][maxln + 10];
unsigned int rev[maxln + 10][maxn + 10], lg[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
static void initLog(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
        fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
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
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
        unit[1][i] = quickPow(unit[0][i], mod - 2);
    }
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 1;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
static void init(const unsigned int n)
{
    const unsigned int lgn = Log2(n) + 2;
    initUnit(lgn);
    initRev(lgn);
    initLog(lgn);
}
void getModInv(const Number a[], Number dest[], const unsigned int n)
{
    static Number prd[maxn + 10], iprd[maxn + 10];
    prd[0] = a[0];
    for (unsigned int i = 1; i < n; ++i)
        prd[i] = prd[i - 1] * a[i];
    iprd[n - 1] = quickPow(prd[n - 1], mod - 2);
    for (unsigned int i = n - 1; i; --i)
    {
        iprd[i - 1] = iprd[i] * a[i];
        dest[i] = iprd[i] * prd[i - 1];
    }
    dest[0] = iprd[0];
}

inline void reverseCopy(Number dest[], ConstArrayPtr src, const unsigned int lgn)
{
    static const Number zero;
    const unsigned int n = 1u << lgn;
    const unsigned int* const crev = rev[lgn];
    for (unsigned int i = 0; i < src.size; ++i)
        dest[crev[i]] = src.ptr[i];
    for (unsigned int i = src.size; i < n; ++i)
        dest[crev[i]] = zero;
}
template <bool inv>
void dft(const ConstArrayPtr& a, Number dest[], const unsigned int lgn)
{
    reverseCopy(dest, a, lgn);
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        const unsigned int step = 1u << i, half = 1u << (i - 1);
        const Number wm = unit[inv][i];
        for (Number* blk = dest; blk < dest + n; blk += step)
        {
            Number w = 1;
            for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k)
            {
                const Number u = *j, v = w * (*k);
                *j = u + v;
                *k = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (inv)
    {
        const Number iv = quickPow(n, mod - 2);
        for (Number* i = dest; i < dest + n; ++i)
            *i *= iv;
    }
}
template <bool op1 = false, bool op2 = false, bool op3 = true, class Func>
void convolution(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr dest, const Func pred)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    const unsigned int lgn = lg[dest.size];
    dest.size = 1u << lgn;
    dft<op1>(a, tmpa, lgn);
    dft<op2>(b, tmpb, lgn);
    for (unsigned int i = 0; i < dest.size; ++i)
        tmpb[i] = pred(tmpa[i], tmpb[i]);
    dft<op3>(ConstArrayPtr { tmpb, dest.size }, dest.ptr, lgn);
}

template <bool op1 = false, bool op2 = false, bool op3 = true>
inline void ntt(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& dest)
{
    dest.size = a.size + b.size - 1;
    convolution<op1, op2, op3>(a, b, dest, multiplies<Number>());
}
inline void updateInverse(const ConstArrayPtr pol, Number dest[], const unsigned int n)
{
    convolution(
        pol,
        ConstArrayPtr { dest, n >> 1 },
        ArrayPtr { dest, n << 1 },
        [](Number a, Number b) { return b * (2 - a * b); });
}
inline void inverse(const ConstArrayPtr& a, ArrayPtr dest)
{
    dest.ptr[0] = quickPow(a.ptr[0], mod - 2);
    const unsigned int lgn = lg[dest.size], n = 1u << lgn, lim = min(n, a.size);
    Number* const pDest = dest.ptr;
    unsigned int i = 2;
    for (; i <= lim; i <<= 1)
        updateInverse(ConstArrayPtr { a.ptr, i }, pDest, i);
    for (; i <= n; i <<= 1)
        updateInverse(a, pDest, i);
}
inline void derivative(const ConstArrayPtr a, ArrayPtr& dest)
{
    for (unsigned int i = 0; i + 1 < a.size; ++i)
        dest.ptr[i] = (i + 1) * a.ptr[i + 1];
    dest.size = a.size - 1;
}

enum
{
    TransMem,
    InterpMem,
};
Number mem[2][maxn * maxln * 4 * 2 + 19], *cur[2] = { mem[0], mem[1] };
ConstArrayPtr prod[2][maxn * 4];

template <unsigned int typ, bool op1 = false, bool op2 = false, bool op3 = true>
inline ConstArrayPtr multiply(const ConstArrayPtr& a, const ConstArrayPtr& b)
{
    ArrayPtr ret { cur[typ], a.size + b.size - 1 };
    convolution<op1, op2, op3>(a, b, ret, multiplies<Number>());
    return ret;
}
template <unsigned int typ>
void productX(const unsigned int cid, const unsigned int l, const unsigned int r, const Number v[])
{
    static ConstArrayPtr* const cprod = prod[typ];
    if (r - l == 1)
    {
        if constexpr (typ == TransMem)
        {
            cur[TransMem][0] = 1;
            cur[TransMem][1] = -v[l];
        }
        else
        {
            cur[InterpMem][0] = -v[l];
            cur[InterpMem][1] = 1;
        }
        cprod[cid] = ConstArrayPtr { cur[typ], 2 };
        cur[typ] += 2;
        return;
    }
    const unsigned int left = cid << 1, right = left + 1, mid = (l + r) >> 1;
    productX<typ>(left, l, mid, v);
    productX<typ>(right, mid, r, v);
    cprod[cid] = multiply<typ>(cprod[left], cprod[right]);
    cur[typ] += cprod[cid].size;
}
namespace Evaluation
{
    ConstArrayPtr* const cprod = prod[TransMem];
    ConstArrayPtr t[maxn * 4 + 10];

    ConstArrayPtr transMultiply(const Number vx[], const ConstArrayPtr& y, const unsigned int lgn)
    {
        static Number tmpb[maxn + 10];
        dft<false>(y, tmpb, lgn);
        const unsigned int n = 1u << lgn;
        for (unsigned int i = 0; i < n; ++i)
            tmpb[i] *= vx[i];
        ArrayPtr ret { cur[TransMem], n };
        dft<false>(ConstArrayPtr { tmpb, n }, cur[TransMem], lgn);
        return ret;
    }
    void evalPoly(const unsigned int cid, const unsigned int l, const unsigned int r, Number dest[])
    {
        if (r - l == 1)
        {
            dest[l] = t[cid].ptr[0];
            return;
        }
        const unsigned int left = cid << 1, right = left + 1, mid = (l + r) >> 1;
        {
            static Number tmpv[maxn + 10];
            const unsigned int lgn = lg[t[cid].size + max(cprod[right].size, cprod[left].size) - 1];
            dft<true>(t[cid], tmpv, lgn);
            t[left] = transMultiply(tmpv, cprod[right], lgn);
            t[left].size = mid - l;
            cur[TransMem] += mid - l;
            t[right] = transMultiply(tmpv, cprod[left], lgn);
            t[right].size = r - mid;
            cur[TransMem] += r - mid;
        }
        evalPoly(left, l, mid, dest);
        evalPoly(right, mid, r, dest);
    }

    void evaluation(const ConstArrayPtr& pol, const ConstArrayPtr& pos, Number dest[])
    {
        cur[TransMem] = mem[TransMem];
        productX<TransMem>(1, 0, pos.size, pos.ptr);
        {
            static Number tmp[maxn + 10];
            ArrayPtr iv { tmp, cprod[1].size }, tp { cur[TransMem], cprod[1].size };
            inverse(cprod[1], iv);
            ntt<false, true, false>(iv, pol, tp);
            tp.size = pos.size;
            cur[TransMem] += tp.size;
            t[1] = tp;
        }
        evalPoly(1, 0, pos.size, dest);
    }
}
namespace Interpolation
{
    ConstArrayPtr f[maxn * 4 + 10];
    ConstArrayPtr* const cprod = prod[InterpMem];

    template <bool isRoot>
    void conquer(const unsigned int cid, const unsigned int l, const unsigned int r, const Number v[])
    {
        if (r - l == 1)
        {
            cur[InterpMem][0] = v[l];
            if (!isRoot)
            {
                f[cid] = ConstArrayPtr { cur[InterpMem], 1 };
                cur[InterpMem] += 1;
            }
            return;
        }
        const unsigned int left = cid << 1, right = left + 1, mid = (l + r) >> 1;
        conquer<false>(left, l, mid, v);
        conquer<false>(right, mid, r, v);
        {
            static Number tmpa[maxn + 10], tmpb[maxn + 10], tmpc[maxn + 10], tmpd[maxn + 10];
            const unsigned int actualLen = max(f[left].size + cprod[right].size, f[right].size + cprod[left].size) - 1;
            const unsigned int lgn = lg[actualLen], n = 1u << lgn;
            dft<false>(f[left], tmpa, lgn);
            dft<false>(cprod[left], tmpb, lgn);
            dft<false>(f[right], tmpc, lgn);
            dft<false>(cprod[right], tmpd, lgn);
            for (unsigned int i = 0; i < n; ++i)
                tmpd[i] = tmpa[i] * tmpd[i] + tmpb[i] * tmpc[i];
            if constexpr (!isRoot)
            {
                f[cid] = ConstArrayPtr { cur[InterpMem], actualLen };
                cur[InterpMem] += actualLen;
            }
            else
                f[cid].size = actualLen;
            dft<true>(ConstArrayPtr { tmpd, n }, const_cast<Number*>(f[cid].ptr), lgn);
        }
    }

    void interpolation(const Number x[], const Number y[], ArrayPtr& dest, const unsigned int n)
    {
        static Number memDx[maxn + 10], v[maxn + 10];
        cur[InterpMem] = mem[InterpMem];
        productX<InterpMem>(1, 0, n, x);
        {
            ArrayPtr dx { memDx };
            derivative(cprod[1], dx);
            Evaluation::evaluation(dx, ConstArrayPtr { x, n }, v);
            getModInv(v, v, n);
            for (unsigned int i = 0; i < n; ++i)
                v[i] *= y[i];
        }
        f[1] = dest;
        conquer<true>(1, 0, n, v);
        dest.size = f[1].size;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    init(n);
    Number x[maxn + 10], y[maxn + 10], ans[maxn + 10];
    for (unsigned int i = 0; i < n; ++i)
        cin >> x[i] >> y[i];
    ArrayPtr dest { ans, 0 };
    Interpolation::interpolation(x, y, dest, n);
    copy(dest.ptr, dest.ptr + dest.size, ostream_iterator<Number>(cout, " "));
    for (unsigned int i = n - dest.size; i; --i)
        cout << "0 ";
    cout.put('\n');
    return 0;
}