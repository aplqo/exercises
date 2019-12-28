#ifdef APDEBUG
#define APINPROG
#define COLOR
#undef _MSC_VER
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
typedef long double fType;
const int maxn = 1005, md = 50000;

class res
{
public:
    res()
        : d(0)
    {
        fill(digit, digit + md, 0);
    };
    res(unsigned int a);
    res(const res& r)
    {
        d = r.d;
        copy(r.digit, r.digit + r.d, digit);
    };
    inline bool operator<(const res& r) const;
    inline res& operator*=(unsigned int r);
    inline res operator/(unsigned int r) const;
    friend ostream& operator<<(ostream& o, res& a);

private:
    inline void fix();
    unsigned int digit[md], d;
};
res::res(unsigned int a)
{
    fill(digit, digit + md, 0);
    d = 0;
    do
    {
        digit[d++] = a % 10;
        a /= 10;
    } while (a);
}
inline bool res::operator<(const res& r) const
{
    if (this->d != r.d)
        return this->d < r.d;
    for (int i = d - 1; i >= 0; --i)
        if (this->digit[i] != r.digit[i])
            return this->digit[i] < r.digit[i];
    return false;
}
inline res& res::operator*=(unsigned int r)
{
    for (unsigned int* i = digit + d - 1; i > digit - 1; --i)
        *i *= r;
    this->fix();
    return *this;
}
inline res res::operator/(unsigned int r) const
{
    res ret;
    unsigned int m = 0;
    for (int i = d - 1; i >= 0; --i)
    {
        unsigned int b = (digit[i] + m) % r;
        ret.digit[i] = (digit[i] + m) / r;
        m = b * 10;
    }
    ret.fix();
    return ret;
}
ostream& operator<<(ostream& o, res& a)
{
    for (int i = a.d - 1; i >= 0; --i)
        o << a.digit[i];
    if (a.d == 0)
        cout << "0";
    return o;
}
inline void res::fix()
{
    for (int i = 0; i < d; ++i)
    {
        unsigned int t = digit[i] / 10;
        digit[i] %= 10;
        digit[i + 1] += t;
    }
    for (d = md - 1; d > 1 && digit[d - 1] == 0; --d)
        ;
    while (digit[d - 1] >= 10)
    {
        digit[d] += digit[d - 1] / 10;
        digit[d - 1] %= 10;
        ++d;
    }
}
struct peo
{
    unsigned int a, b;
} p[maxn];
res ans(0);

bool cmp(const peo& a, const peo& b)
{
    return max(fType(1) / a.b, fType(a.a) / b.b) < max(fType(1) / b.b, fType(b.a) / a.b);
}
int main()
{
    unsigned int n;
    cin >> n;
    for (peo* i = p; i < p + n + 1; i++)
        cin >> i->a >> i->b;
    sort(p + 1, p + 1 + n, cmp);
    res t(p[0].a);
    for (peo* i = p + 1; i < p + 1 + n; i++)
    {
#ifdef APDEBUG_comp
        cout << (t / i->b) << endl;
#endif
        ans = max(ans, t / i->b);
        t *= i->a;
    }
    cout << ans;
    return 0;
}
