#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 200000;
constexpr unsigned long long mod[] = { (unsigned long long)1e9 + 7, (unsigned long long)1e9 + 9 }, base = 7;

char str[maxl + 10];
unsigned long long po[2][maxl + 1];
unsigned long long has[2][26][maxl + 1];

template <unsigned long long mod>
static void getPow(const unsigned int l, unsigned long long res[])
{
    res[0] = 1;
    for (unsigned long long* i = res + 1; i < res + 1 + l; ++i)
        *i = (*(i - 1) * base) % mod;
}
template <unsigned long long mod>
void getHash(const char* str, const unsigned int len, unsigned long long res[26][maxl + 1])
{
    for (unsigned int i = 1; i <= len; ++i)
    {
        for (unsigned int j = 0; j < 26; ++j)
            res[j][i] = (res[j][i - 1] * base) % mod;
        const unsigned int ch = str[i] - 'a';
        res[ch][i] = (res[ch][i] + 1) % mod;
    }
}
template <unsigned int typ>
inline unsigned long long subHash(const unsigned int ch, const unsigned int l, const unsigned int r) // [l,r]
{
    return (has[typ][ch][r] + mod[typ] - (has[typ][ch][l - 1] * po[typ][r + 1 - l]) % mod[typ]) % mod[typ];
}
static void init(const unsigned int n)
{
    getHash<mod[0]>(str, n, has[0]);
    getHash<mod[1]>(str, n, has[1]);
    getPow<mod[0]>(n, po[0]);
    getPow<mod[1]>(n, po[1]);
}
bool check(const unsigned int pl, const unsigned int pr, const unsigned int len)
{
    typedef pair<unsigned long long, unsigned long long> Hash;
    Hash hl[27], hr[27];
    for (unsigned int i = 0; i < 26; ++i)
    {
        hl[i] = Hash { subHash<0>(i, pl, pl + len - 1), subHash<1>(i, pl, pl + len - 1) };
        hr[i] = Hash { subHash<0>(i, pr, pr + len - 1), subHash<1>(i, pr, pr + len - 1) };
    }
    sort(hl, hl + 26);
    sort(hr, hr + 26);
    return equal(hl, hl + 26, hr);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m >> (str + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y, z;
        cin >> x >> y >> z;
        cout << (check(x, y, z) ? "YES" : "NO") << "\n";
    }
    return 0;
}