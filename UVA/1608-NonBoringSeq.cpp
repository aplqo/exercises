#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;
constexpr int maxn = 200010;

int a[maxn], pre[maxn], nxt[maxn];
int n = maxn;
map<int, int> lst;

bool fun(int beg, int end)
{
    if (end - beg <= 1)
        return true;
    auto test = [&beg, &end](int p) -> bool {
        return pre[p] < beg && nxt[p] >= end;
    };
    int i = beg, j = end - 1;
    while (i <= j && (!test(i)) && (!test(j)))
    {
        ++i;
        --j;
    }
    if (i <= j)
    {
        if (test(i) && test(j))
            return fun(beg, i) && fun(i + 1, j) && fun(j + 1, end);
        if (test(i))
            return fun(beg, i) && fun(i + 1, end);
        if (test(j))
            return fun(beg, j) && fun(j + 1, end);
    }
    return false;
}
void solve()
{
    cin >> n;
    for (int* i = a; i < a + n; ++i)
        cin >> *i;
    for (int i = 0; i < n; ++i)
    {
        auto it = lst.find(a[i]);
        if (it == lst.end())
            lst[a[i]] = i;
        else
        {
            pre[i] = it->second;
            nxt[it->second] = i;
            it->second = i;
        }
    }
    cout << (fun(0, n) ? "non-boring" : "boring") << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i)
    {
        fill(pre, pre + n, -1);
        fill(nxt, nxt + n, maxn);
        solve();
        lst.~map();
        new (&lst) decltype(lst);
    }
    return 0;
}