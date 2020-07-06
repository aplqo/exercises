#ifdef APTEST
#include "debug_tools/program.h"
#endif
#pragma GCC target("avx512vpopcntdq")
#pragma GCC optimize("Ofast,tree-loop-vectorize,tree-loop-optimize")
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
using namespace std;
const unsigned int maxn = 100000;

unsigned int a[maxn + 1];
unsigned int n;

unsigned long long query(const unsigned int x, const unsigned int y)
{
    unsigned long long ret = 0;
    for (unsigned int i = 0; i < n; ++i)
        ret += (a[i] + x) & y;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    stringstream ibuf;
    ostringstream obuf;
    unsigned int q;
    ibuf << cin.rdbuf();
    ibuf >> n >> q;
    copy_n(istream_iterator<unsigned int>(ibuf), n, a);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int op, x, y;
        ibuf >> op >> x >> y;
        if (op == 1)
            a[x - 1] = y;
        else
            obuf << query(x, y) << endl;
    }
    cout << obuf.str();
    return 0;
}