#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 50000, maxl = 17;

unsigned int lg[maxn + 1];
unsigned int h[maxn];
template <class T>
struct table
{
    void init(const unsigned int n)
    {
        for (unsigned int i = 0; i < n; ++i)
            f[i][0] = h[i];
        for (unsigned int i = 1; i <= lg[n]; ++i)
            for (unsigned int j = 0; j + (1 << i) <= n; ++j)
                f[j][i] = max(f[j][i - 1], f[j + (1 << (i - 1))][i - 1], T());
    }
    unsigned int query(const unsigned int l, const unsigned int r) const
    {
        const unsigned int d = lg[r - l + 1];
        return max(f[l][d], f[r + 1 - (1 << d)][d], T());
    }

private:
    unsigned int f[maxn + 1][maxl + 1];
};
table<less<unsigned int>> gt;
table<greater<unsigned int>> lt;

static void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
static void init(const unsigned int n)
{
    getLog(n);
    lt.init(n);
    gt.init(n);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    copy_n(istream_iterator<unsigned int>(cin), n, h);
    init(n);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        --a;
        --b;
        cout << (gt.query(a, b) - lt.query(a, b)) << endl;
    }
    return 0;
}