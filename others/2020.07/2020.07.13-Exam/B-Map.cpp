#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
using namespace std;
const unsigned long long mod = 998244353;

class Number
{
public:
    Number(const unsigned long long v = 0)
        : val(v % mod)
    {
    }
#define decl(x)                                                      \
    inline friend Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
#undef decl

    unsigned long long val;
};
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(+);
impl(*);
#undef impl
Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret *= a;
            e ^= i;
        }
        a *= a;
    }
    return ret;
}
struct vec
{
    vec() {}
    vec(Number v1, Number v2)
    {
        val[0] = v1;
        val[1] = v2;
    }
    Number val[2];
};
struct matrix
{
    matrix(vec v1, vec v2)
    {
        col[0] = v1;
        col[1] = v2;
    }
    vec col[2];
};
vec operator*(const matrix& l, const vec& r)
{
    vec ret(0, 0);
    for (unsigned int i = 0; i < 2; ++i)
        for (unsigned int j = 0; j < 2; ++j)
            ret.val[j] += r.val[i] * l.col[i].val[j];
    return ret;
}
matrix operator*(const matrix& l, const matrix& r)
{
    matrix ret(vec(0, 0), vec(0, 0));
    for (unsigned int i = 0; i < 2; ++i)
        ret.col[i] = r * l.col[i];
    return ret;
}
matrix operator^(matrix v, unsigned long long e)
{
    matrix ret(vec(1, 0), vec(0, 1));
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret = ret * v;
            e ^= i;
        }
        v = v * v;
    }
    return ret;
}

int main()
{
    unsigned long long n;
    unsigned int q;
    scanf("%llu%u", &n, &q);
    const Number inv = quickPow(n - 1, mod - 2);
    const matrix m(vec(0, inv), vec(1, inv * (n - 2)));
    const vec iv(0, inv);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned long long t;
        scanf("%llu", &t);
        switch (t)
        {
        case 0:
            printf("%d\n", 1);
            break;
        case 1:
            printf("%d\n", 0);
            break;
        default:
            printf("%lld\n", ((m ^ (t - 2)) * iv).val[1].val);
        }
    }
    return 0;
}
