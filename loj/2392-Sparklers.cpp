#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
using namespace std;
const unsigned int maxn = 1e5;

int x[maxn + 10];
long long p[maxn + 10];

void generateP(const int n, const long long t, const int v)
{
    const long long bas = 2 * t * v;
    for (int i = 1; i <= n; ++i)
        p[i] = x[i] - bas * i;
}
template <bool lr, bool dir> // lr: true->l, false->r dir: true->increase,false->decrease
int expand(int l, const int g, const long long lim) // init pos, GL/GR, r/l
{
    static conditional_t<lr, greater_equal<long long>, less_equal<long long>> upd, chk; // check p[l] >= p[r]
    static conditional_t<dir, plus<int>, minus<int>> nxt; // move to next pos
    static conditional_t<dir, less_equal<int>, greater_equal<int>> brd; // not exceed GL/GR
    for (int i = nxt(l, 1); brd(i, g) && chk(p[i], lim); i = nxt(i, 1))
        if (upd(p[i], p[l]))
            return i;
    return l;
}
template <bool dir>
bool checkGLR(int l, int r, const int gl, const int gr)
{
    if (p[l] < p[r])
        return false;
    while (true)
    {
        const int nl = expand<true, !dir>(l, gl, p[r]);
        const int nr = expand<false, dir>(r, gr, p[nl]);
        if (nl == l && nr == r)
            break;
        l = nl;
        r = nr;
    }
    return gl == l && gr == r;
}
bool check(const unsigned int n, const unsigned int k, const int t, const int v)
{
    generateP(n, t, v);
    const int gl = max_element(p + 1, p + 1 + k) - p, gr = min_element(p + k, p + n + 1) - p;
    return checkGLR<false>(1, n, gl, gr) && checkGLR<true>(k, k, gl, gr);
}
int binarySearch(const int n, const int t, const int k)
{
    int l = 0, r = x[n];
    while (l <= r)
    {
        const int mid = (l + r) / 2;
        if (check(n, k, t, mid))
            r = mid - 1;
        else
            l = mid + 1;
    }
    return l;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k, t;
    cin >> n >> k >> t;
    copy_n(istream_iterator<int>(cin), n, x + 1);
    if (all_of(x + 1, x + 1 + n, [](long long i) { return i == x[1]; }))
    {
        cout << "0\n";
        return 0;
    }
    cout << binarySearch(n, t, k) << "\n";
    return 0;
}