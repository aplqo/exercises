#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned long long b = 131, mod = 998244353;

class StackHash
{
public:
    void push(const char c)
    {
        has[ptr + 1] = has[ptr] * b + c;
        stk[++ptr] = c;
    }
    inline void pop() { --ptr; }
    inline bool empty() const { return !ptr; }
    inline char top() const { return stk[ptr]; }
    inline unsigned long long hash() const { return has[ptr]; }

private:
    char stk[maxn + 1];
    unsigned long long has[maxn + 1] = {};
    unsigned int ptr = 0;
};
char str[maxn + 10];
unsigned long long h[maxn + 10];

void GetHash(const size_t n)
{
    static StackHash hstk;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!hstk.empty() && hstk.top() == str[i])
            hstk.pop();
        else
            hstk.push(str[i]);
        h[i] = hstk.hash();
    }
}
void discretize(const size_t len)
{
    static unsigned long long tmp[maxn + 10];
    copy(h, h + len, tmp);
    sort(tmp, tmp + len + 1);
    unsigned long long* const e = unique(tmp, tmp + len + 1);
    for (unsigned long long* i = h; i < h + len; ++i)
        *i = lower_bound(tmp, e, *i) - tmp;
}
unsigned long long count(const size_t len)
{
    static unsigned int cnt[maxn + 10];
    unsigned long long ret = 0;
    ++cnt[0];
    for (unsigned int i = 0; i < len; ++i)
    {
        ret += cnt[h[i]];
        ++cnt[h[i]];
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> str;
    const size_t len = strlen(str);
    GetHash(len);
    discretize(len);
    cout << count(len) % mod << "\n";
    return 0;
}