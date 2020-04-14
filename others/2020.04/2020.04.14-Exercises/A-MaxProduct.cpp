/*
* Luogu team T129620: max product
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 40, maxk = 6, maxl = maxn * maxn;

class Number
{
public:
    Number() = default;
    Number(unsigned int d[], unsigned int l)
    {
        copy(d, d + l, dat);
        cur = l;
    }
    inline Number operator*(const Number& r) const
    {
        Number ret;
        for (unsigned int i = 0; i < cur; ++i)
            for (unsigned int j = 0; j < r.cur; ++j)
                ret.dat[i + j] += dat[i] * r.dat[j];
        ret.fix();
        return ret;
    }
    inline bool operator<(const Number& r) const
    {
        if (cur != r.cur)
            return cur < r.cur;
        for (unsigned int i = cur; i > 0; --i)
            if (dat[i - 1] != r.dat[i - 1])
                return dat[i - 1] < r.dat[i - 1];
        return false;
    }
    inline friend ostream& operator<<(ostream& os, const Number& v);

private:
    void fix()
    {
        for (unsigned int i = 0; i < maxl; ++i)
        {
            dat[i + 1] += dat[i] / 10;
            dat[i] %= 10;
        }
        for (cur = maxl + 1; cur > 0 && !dat[cur - 1]; --cur)
            ;
    }

    unsigned int dat[maxl + 10] = {}, cur = 0;
};
inline ostream& operator<<(ostream& os, const Number& v)
{
    for (unsigned int i = v.cur; i; --i)
        os << v.dat[i - 1];
    return os;
}

Number f[maxn + 10][maxk + 10];
unsigned int dig[maxn + 10];

void init(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        f[i][0] = Number(dig + 1, i);
}
void dp(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= min(i - 1, k); ++j)
        {
            f[i][j] = f[i - 1][j];
            for (unsigned int k = j + 1; k <= i; ++k)
                f[i][j] = max(f[i][j], f[k - 1][j - 1] * Number(dig + k, i - k + 1));
        }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    cin.ignore(2, '\n');
    for (unsigned int i = n; i; --i)
        dig[i] = cin.get() - '0';
    init(n);
    dp(n, k);
    cout << f[n][k] << endl;
    return 0;
}