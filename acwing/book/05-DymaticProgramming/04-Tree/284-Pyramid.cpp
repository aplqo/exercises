#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
using num = unsigned long long;
constexpr num mod = 1e9;
const int maxl = 300;

class Number
{
public:
    Number() = default;
    Number(const num v)
        : dat(v % mod)
    {
    }
#define aop(x)                                                                    \
    Number operator x(const Number r) const { return Number(this->dat x r.dat); } \
    Number& operator x##=(const Number r) { return *this = *this x r; }
    aop(*);
    aop(+);
#undef aop
    friend ostream& operator<<(ostream& os, const Number v);

private:
    num dat = 0;
};
ostream& operator<<(ostream& os, const Number v)
{
    os << v.dat;
    return os;
}

Number f[maxl + 10][maxl + 10];
char str[maxl + 10];
size_t len;

void init()
{
    len = strlen(str);
    for (unsigned int i = 0; i < len; ++i)
        f[i][i] = 1;
}
Number dp()
{
    for (unsigned int i = 1; i < len; ++i)
        for (unsigned int j = 0; j + i < len; ++j)
        {
            if (str[j] != str[j + i])
                continue;
            f[j][j + i] += f[j + 1][j + i - 1];
            for (unsigned int k = j + 1; k < j + i; ++k)
                if (str[k] == str[j])
                    f[j][j + i] += f[j + 1][k - 1] * f[k][j + i];
        }
    return f[0][len - 1];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> str;
    init();
    cout << dp() << endl;
    return 0;
}