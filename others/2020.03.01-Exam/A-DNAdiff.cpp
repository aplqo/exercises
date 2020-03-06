#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
using namespace std;
using hash_t = unsigned long long;
const int maxn = 100;

class mystring
{
public:
    void InitHash()
    {
        length = strlen(str + 1);
        for (int i = 1; i <= length; ++i)
            has[i] = has[i - 1] * b + str[i];
    }
    hash_t Gethash(const int beg, const int end) const
    {
        return has[end] - has[beg - 1] * po[end - beg + 1];
    }
    int Distance(const int pos)
    {
        return length - pos + 1;
    }
    static void GetPow()
    {
        po[0] = 1;
        for (int i = 1; i <= maxn; ++i)
            po[i] = po[i - 1] * b;
    }
    char str[maxn + 10];
    int length;

private:
    static hash_t po[maxn + 10];
    static constexpr hash_t b = 131;
    hash_t has[maxn + 10];
} t1, t2;
hash_t mystring::po[maxn + 10];
int mem[maxn + 10][maxn + 10];
bool vis[maxn + 10][maxn + 10];

inline int One(const int p1, const int p2)
{
    static const auto check = [](int beg, const mystring& s, char c) -> int {
        return find(s.str + beg, s.str + t2.length + 1, c) != s.str + s.length + 1;
    };
    if (t1.Distance(p1) == 1 && t2.Distance(p2) == 1)
        return mem[p1][p2] = t1.str[p1] == t2.str[p2] ? 1 : 0;
    else if (t1.Distance(p1) == 1)
        return (t2.Distance(p2) - 1) * (-2) + check(p2, t2, t1.str[p1]);
    else
        return (t1.Distance(p1) - 1) * (-2) + check(p1, t1, t2.str[p2]);
}
int fun(const int p1, const int p2)
{
    if (t1.Distance(p1) == 1 || t2.Distance(p2) == 1)
        return One(p1, p2);
    else if (p1 > t1.length && p2 > t2.length)
        return 0;
    else if (p1 > t1.length)
        return t2.Distance(p2) * (-2);
    else if (p2 > t2.length)
        return t1.Distance(p1) * (-2);
    if (vis[p1][p2])
        return mem[p1][p2];
    vis[p1][p2] = true;
    if (t1.str[p1] == t2.str[p2])
        return mem[p1][p2] = fun(p1 + 1, p2 + 1) + 1;
    else
        return mem[p1][p2] = max({ fun(p1 + 1, p2 + 1), fun(p1 + 1, p2) - 2, fun(p1, p2 + 1) - 2 });
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    mystring::GetPow();
    int ans = INT_MIN;
    cin >> (t1.str + 1) >> (t2.str + 1);
    t1.InitHash();
    t2.InitHash();
    if (t1.length != t2.length)
    {
        const int end = max(t1.length, t2.length) - min(t1.length, t2.length) + 1;
        for (int i = 1; i <= end; ++i)
            ans = max(ans, fun(1, i) + (i - 1) * (-2));
    }
    else
        ans = fun(1, 1);
    cout << ans << endl;
    return 0;
}
