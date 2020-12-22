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
    inline void operator x##=(const Number r) { *this = *this x r; }
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

private:
    unsigned long long val = 0;
};
Number unit[2][maxln + 10];
unsigned int rev[maxn + 10], lg[maxn + 10];
template <class T>
struct Pointer
{
    T* const ptr;
    unsigned int size;
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;

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
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
        unit[1][i] = quickPow(unit[0][i], mod - 2);
        lg[1u << i] = i;
    }
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
static void initRev(const unsigned int lgn)
{
    for (unsigned int i = 0; i < lgn; ++i)
        rev[1u << i] = 1u << (lgn - 1 - i);
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int lb = i & -i;
        rev[i] = rev[i ^ lb] | rev[lb];
    }
}
void reverseCopy(ArrayPtr dest, const ConstArrayPtr src)
{
    static const Number zero;
    for (unsigned int i = 0; i < src.size; ++i)
        dest.ptr[rev[i]] = src.ptr[i];
    for (unsigned int i = src.size; i < dest.size; ++i)
        dest.ptr[rev[i]] = zero;
}
template <unsigned int typ>
void dft(const ConstArrayPtr a, ArrayPtr dest)
{
    reverseCopy(dest, a);
    for (unsigned int step = 2; step <= dest.size; step <<= 1)
    {
        const unsigned int half = step >> 1;
        const Number wm = unit[typ][lg[step]];
        for (unsigned int blk = 0; blk < dest.size; blk += step)
        {
            Number w = 1;
            for (unsigned int i = 0; i < half; ++i)
            {
                const Number u = dest.ptr[blk + i], v = w * dest.ptr[blk + half + i];
                dest.ptr[blk + i] = u + v;
                dest.ptr[blk + half + i] = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (typ)
    {
        const Number iv = quickPow(dest.size, mod - 2);
        for (Number* i = dest.ptr; i < dest.ptr + dest.size; ++i)
            *i *= iv;
    }
}
template <class Func>
void convolution(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr dest, Func pred)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dest.size = 1u << Log2(dest.size);
    initRev(lg[dest.size]);
    dft<0>(a, ArrayPtr { tmpa, dest.size });
    dft<0>(b, ArrayPtr { tmpb, dest.size });
    for (unsigned int i = 0; i < dest.size; ++i)
        tmpb[i] = pred(tmpa[i], tmpb[i]);
    dft<1>(ConstArrayPtr { tmpb, dest.size }, dest);
}

inline void ntt(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& dest)
{
    dest.size = a.size + b.size - 1;
    convolution(a, b, dest, [](Number a, Number b) { return a * b; });
}
void inverse(const ConstArrayPtr& a, ArrayPtr& dest)
{
    dest.ptr[0] = quickPow(a.ptr[0], mod - 2);
    const unsigned int sz = 1u << Log2(a.size);
    for (unsigned int i = 1; (1u << i) <= sz; ++i)
        convolution(
            ConstArrayPtr { a.ptr, 1u << i },
            ConstArrayPtr { dest.ptr, 1u << (i - 1) },
            ArrayPtr { dest.ptr, 1u << (i + 1) },
            [](Number a, Number b) { return b * (2 - a * b); });
    dest.size = a.size;
}
void divide(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& div, ArrayPtr& mod)
{
    static Number tmpc[maxn + 10], rev[maxn + 10];
    const unsigned int dsize = a.size + 1 - b.size;
    ArrayPtr iv { tmpc, b.size };
    {
        reverse_copy(b.ptr, b.ptr + b.size, rev);
        inverse(ConstArrayPtr { rev, dsize + 1 }, iv);
    }
    reverse_copy(a.ptr, a.ptr + a.size, rev);
    ntt(ConstArrayPtr { rev, dsize + 1 }, ConstArrayPtr { iv.ptr, iv.size }, div);
    div.size = dsize;
    reverse(div.ptr, div.ptr + div.size);

    {
        ArrayPtr iv { tmpc };
        ntt(ConstArrayPtr { div.ptr, div.size }, b, iv);
        mod.size = b.size - 1;
        for (unsigned int i = 0; i < mod.size; ++i)
            mod.ptr[i] = a.ptr[i] - tmpc[i];
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    static Number a[maxn + 10], b[maxn + 10], d[maxn + 10], mo[maxn + 10];
    unsigned int n, m;
    cin >> n >> m;
    ++n;
    ++m;
    copy_n(istream_iterator<Number>(cin), n, a);
    copy_n(istream_iterator<Number>(cin), m, b);
    initUnit(Log2(n) + 1);
    ArrayPtr pd { d, 0 }, pm { mo, 0 };
    divide(ConstArrayPtr { a, n }, ConstArrayPtr { b, m }, pd, pm);
    copy(d, d + pd.size, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    copy(mo, mo + pm.size, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    return 0;
}