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
    int distance(const int pos)
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
    const char* p;
    char c;
    int beg, end;
    int ret = 0;
    if (t1.distance(p1) == 1 && t2.distance(p2) == 1)
        return mem[p1][p2] = t1.str[p1] == t2.str[p2] ? 1 : 0;
    else if (t1.distance(p1) == 1)
    {
        c = t1.str[p1];
        beg = p2;
        end = t2.length;
        p = t2.str;
        ret = (t2.distance(p2) - 1) * (-2);
    }
    else
    {
        c = t2.str[p2];
        beg = p1;
        end = t1.length;
        p = t1.str;
        ret = (t1.distance(p1) - 1) * (-2);
    }
    for (; beg <= end; ++beg)
        if (p[beg] == c)
        {
            ++ret;
            break;
        }
    return ret;
}
int fun(const int p1, const int p2)
{
    if (t1.distance(p1) == 1 || t2.distance(p2) == 1)
        return One(p1, p2);
    else if (p1 > t1.length && p2 > t2.length)
        return 0;
    else if (p1 > t1.length)
        return t2.distance(p2) * (-2);
    else if (p2 > t2.length)
        return t1.distance(p1) * (-2);
    if (vis[p1][p2])
        return mem[p1][p2];
    vis[p1][p2] = true;

    const int d = min(t1.distance(p1), t2.distance(p2));
    int l = 0, r = d - 1;
    while (l <= r)
    {
        int m = (l + r) >> 1;
        if (t1.Gethash(p1, p1 + m) == t2.Gethash(p2, p2 + m))
            l = m + 1;
        else
            r = m - 1;
    }
    if (l != d)
        return mem[p1][p2] = max({ fun(p1 + l, p2 + l + 1) - 2, fun(p1 + l + 1, p2 + l) - 2, fun(p1 + l + 1, p2 + l + 1) }) + l;
    else
        return mem[p1][p2] = (max(t1.distance(p1), t2.distance(p2)) - d) * (-2) + l;
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
            ans = max(ans, fun(i, i));
    }
    else
        ans = fun(1, 1);
    cout << ans << endl;
    return 0;
}
