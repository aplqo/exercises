#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <cctype>
using namespace std;
const unsigned int maxn = 100, maxm = 1e6;
constexpr unsigned int cnt = 2;
constexpr unsigned long long mod[cnt] = { static_cast<unsigned long long>(1e9 + 7), static_cast<unsigned long long>(1e9 + 9) };

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val1(v % mod[0])
        , val2(v % mod[1]) {};
#define decl(x)                                                                                                                                \
    friend inline Number operator x(const Number l, const Number r) { return Number((l.val1 + mod[0]) x r.val1, (l.val2 + mod[1]) x r.val2); } \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
    decl(-);
#undef decl
    inline Number operator-() const
    {
        return 0 - *this;
    }
    friend inline bool operator==(const Number l,const Number r)
    {
        return l.val1 == r.val1 && l.val2 == r.val2;
    }

private:
    Number(const unsigned long long t1, const unsigned long long t2)
        : val1(t1 % mod[0])
        , val2(t2 % mod[1]) {};
    unsigned long long val1, val2;
};
Number a[maxn + 1];
bool sol[maxm + 1];

template<class T>
T readNum()
{
    T ret = 0;
    bool neg = false;
    char c = getchar();
    while(!isdigit(c) && c != '-')
        c = getchar();
    if(c == '-')
    {
        neg = true;
        c = getchar();
    }
    while(isdigit(c))
    {
        ret = ret * 10 + (c - '0');
        c = getchar();
    }
    return neg? -ret: ret;
}
Number eval(const unsigned int n, const unsigned long long x)
{
    const Number nx(x);
    Number ret = a[n];
    for(unsigned int i = n; i > 0; --i)
        ret = ret * nx + a[i - 1];
    return ret;
}
unsigned int solve(const unsigned int n, const unsigned int m)
{
    unsigned int ret = 0;
    for(unsigned int i = 1; i <= m ;++i)
        if(eval(n, i) == 0)
        {
            sol[i] = true;
            ++ret;
        }
    return ret;
}
int main()
{
    const unsigned int n = readNum<unsigned int>(), m =readNum<unsigned int>();
    for(unsigned int i = 0; i <= n; ++i)
        a[i] = readNum<Number>();
    printf("%u\n", solve(n, m));
    for(unsigned int i = 1; i <= m; ++i)
        if(sol[i])
            printf("%u\n", i);
    return 0;
}
    
