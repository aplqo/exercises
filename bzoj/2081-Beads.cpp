#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
using namespace std;
const unsigned int maxn = 2e5;
constexpr unsigned long long m1 = 1e9 + 7, m2 = 1e9 + 9, base = 100003;

struct HashValue
{
    inline HashValue operator+(const unsigned int r) const { return HashValue { (v1 + r) % m1, (v2 + r) % m2 }; }
    inline HashValue operator*(const unsigned int v) const { return HashValue { (v1 * v) % m1, (v2 * v) % m2 }; }
    inline HashValue operator*(const HashValue v) const { return HashValue { (v1 * v.v1) % m1, (v2 * v.v2) % m2 }; }
    inline HashValue operator-(const HashValue& r) const { return HashValue { (v1 + m1 - r.v1) % m1, (v2 + m2 - r.v2) % m2 }; }
    inline bool operator<(const HashValue& r) const { return v1 == r.v1 ? v2 < r.v2 : v1 < r.v1; };
    inline bool operator==(const HashValue& r) const { return v1 == r.v1 && v2 == r.v2; }

    unsigned long long v1, v2;
};
HashValue po[maxn + 10], has[maxn + 10], rhas[maxn + 10];
unsigned int cnt[maxn + 10], str[maxn + 10];

static void getPow(const unsigned int n)
{
    po[0] = HashValue { 1ull, 1ull };
    for (unsigned int i = 1; i <= n; ++i)
        po[i] = po[i - 1] * base;
}
template <bool dir>
void getHash(const unsigned int str[], const size_t len, HashValue res[])
{
    static typename conditional<dir, plus<unsigned int>, minus<unsigned int>>::type nxt;
    size_t lst = dir ? 0 : len + 1;
    for (size_t i = nxt(lst, 1); 0 < i && i <= len; i = nxt(i, 1))
    {
        res[i] = res[lst] * base + str[i];
        lst = i;
    }
}
template <bool dir>
HashValue subHash(const HashValue has[], const unsigned int l, const unsigned int r)
{
    if (dir)
        return has[r] - has[l - 1] * po[r + 1 - l];
    else
        return has[l] - has[r + 1] * po[r + 1 - l];
}

unsigned int count(const unsigned int n, const unsigned int k)
{
    static HashValue tmp[maxn + 10];
    HashValue* cur = tmp;
    for (unsigned int i = 1; i + k <= n + 1; i += k)
        *(cur++) = min(subHash<true>(has, i, i + k - 1), subHash<false>(rhas, i, i + k - 1));
    sort(tmp, cur);
    return unique(tmp, cur) - tmp;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, str + 1);
    getPow(n);
    getHash<true>(str, n, has);
    getHash<false>(str, n, rhas);
    unsigned int mv = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        cnt[i] = count(n, i);
        mv = max(mv, cnt[i]);
    }
    cout << mv << " " << count(cnt + 1, cnt + 1 + n, mv) << "\n";
    for (unsigned int i = 1; i <= n; ++i)
        if (cnt[i] == mv)
            cout << i << " ";
    cout.put('\n');
    return 0;
}