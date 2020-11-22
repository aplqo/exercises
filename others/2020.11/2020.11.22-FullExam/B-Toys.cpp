#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e6;
constexpr unsigned long long inf = ULLONG_MAX / 2;

unsigned int nxt[maxn + 10];

unsigned long long solve(const unsigned int l, const unsigned int b, const unsigned int n)
{
    unsigned int back = 1, front = l, head = b, lst = 0;
    unsigned long long ret = 0;
    while (front < n)
    {
        while (front < n && nxt[front] <= head)
        {
            if (!nxt[front])
                return inf;
            front = nxt[front];
            back = nxt[back];
        }
        if (front == lst)
            return inf;
        lst = front;
        ret += l;
        const unsigned int d = min(b - (head + 1 - back), n - head);
        head += d;
        ret += d;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, b, l;
    cin >> l >> b >> n;
    {
        unsigned int lst = 0;
        for (unsigned int i = 1; i <= n; ++i)
        {
            char ch;
            cin >> ch;
            if (ch == '1')
            {
                nxt[lst] = i;
                lst = i;
            }
        }
    }
    const unsigned long long res = solve(l, b, n);
    if (res == inf)
        cout << "IMPOSSIBLE\n";
    else
        cout << res << "\n";
    return 0;
}