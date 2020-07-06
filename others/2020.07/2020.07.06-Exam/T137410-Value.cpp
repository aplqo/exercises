#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000;
constexpr int ninf = INT_MIN / 2;

struct object
{
    int v, w;
} obj[maxn + 1];
int f[maxn + 1][maxn + 1] = {};

static void init(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        fill(f[i] + 1, f[i] + n + 1, ninf);
}
int solve(const int n)
{
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j < i + 1; ++j)
            f[i][j] = max(f[i - 1][j], f[i - 1][j - 1] + obj[i].v - (j - 1) * obj[i].w);
    int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ret = max(ret, *max_element(f[i], f[i] + i + 1));
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    int n;
    cin >> n;
    for (object* i = obj + 1; i < obj + n + 1; ++i)
        cin >> i->v >> i->w;
    sort(obj + 1, obj + n + 1, [](const object& a, const object& b) { return a.w > b.w; });
    init(n);
    cout << solve(n) << endl;
    return 0;
}
