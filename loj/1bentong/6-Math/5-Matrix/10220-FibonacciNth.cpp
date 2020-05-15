#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <array>
#include <iostream>
using namespace std;
using num = unsigned long long;
num mod = 1e9 + 10;

class Number
{
public:
    Number() = default;
    Number(num i)
        : val(i % mod)
    {
    }
#define decl(x)                                                      \
    inline friend Number operator x(const Number l, const Number r); \
    inline Number operator x##=(const Number r) { return *this = *this x r; }
    decl(*);
    decl(+);
#undef decl
    inline friend ostream& operator<<(ostream& os, const Number r);

private:
    num val = 0;
};
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(*);
impl(+);
#undef impl
inline ostream& operator<<(ostream& os, const Number r)
{
    return os << r.val;
}
using vec = array<Number, 2>;
struct matrix
{
    matrix() = default;
    vec operator*(const vec r) const
    {
        vec ret { 0, 0 };
        for (unsigned int i = 0; i < 2; ++i)
            for (unsigned int j = 0; j < 2; ++j)
                ret[j] += r[i] * val[i][j];
        return ret;
    }
    matrix operator*(const matrix r) const
    {
        matrix ret { { { 0, 0 }, { 0, 0 } } };
        for (unsigned int i = 0; i < 2; ++i)
            ret.val[i] = r * val[i];
        return ret;
    }
    matrix operator^(unsigned int e) const
    {
        matrix ret, v = *this;
        for (unsigned int i = 1; e; i <<= 1)
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

    vec val[2] = { { 1, 0 }, { 0, 1 } };
};
const matrix m { { { 0, 1 }, { 1, 1 } } };

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> mod;
    cout << ((m ^ (n - 2)) * vec { 1, 1 })[1] << endl;
    return 0;
}