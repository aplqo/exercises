#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::exit;
using std::fill;
using std::ios_base;
using std::isspace;
using std::strlen;
const int maxn = 200;

unsigned int n;
char str[maxn + 10], ans[maxn + 10];
struct replace
{
    replace() = default;
    replace(const unsigned int n)
    {
        for (unsigned int i = 0; i < n; ++i)
            to[i] = i;
    }
    unsigned int to[maxn + 10];

    void apply(char* src, char* dest)
    {
        unsigned int l = strlen(src);
        fill(src + l, src + n, ' ');
        for (unsigned int i = 0; i < n; ++i)
            dest[to[i]] = src[i];
        dest[n] = '\0';
    }
    inline unsigned int& operator[](const unsigned int i)
    {
        return to[i];
    }
    inline unsigned int operator[](const unsigned int i) const
    {
        return to[i];
    }
};
replace operator*(const replace& l, const replace& r)
{
    replace ret;
    for (unsigned int i = 0; i < n; ++i)
        ret[i] = r[l[i]];
    return ret;
}
replace operator^(replace l, unsigned int e)
{
    replace ret(n);
    unsigned int t = 1;
    for (; e; t <<= 1)
    {
        if (e & t)
        {
            ret = ret * l;
            e ^= t;
        }
        l = l * l;
    }
    return ret;
}

void solve()
{
    unsigned int l;
    replace v;
    cin >> n;
    if (!n)
        exit(0);
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> v[i];
        --v[i];
    }
    while (true)
    {
        unsigned int k;
        cin >> k;
        if (!k)
            break;
        cin.ignore(2, ' ');
        fill(str, str + n, ' ');
        cin.getline(str, maxn);
        (v ^ k).apply(str, ans);
        cout << ans << endl;
    }
    cout << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (true)
        solve();
    return 0;
}