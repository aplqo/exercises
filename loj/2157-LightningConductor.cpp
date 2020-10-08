#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
using namespace std;
const unsigned int maxn = 5e5;
typedef double real;

unsigned int a[maxn + 10];
real mx[2][maxn + 10];

inline real cost(const int i, const int j)
{
    return a[i] + (sqrt(real(abs(j - i))));
}
template <bool dir>
void conquer(const unsigned int pl, const unsigned int pr, const unsigned int vl, const unsigned int vr)
{
    if (pl > pr)
        return;
    static conditional_t<dir, plus<unsigned int>, minus<unsigned int>> nxt;
    static conditional_t<dir, less<unsigned int>, greater<unsigned int>> cmp;
    const unsigned int mid = (pl + pr) >> 1;
    unsigned int pos = mid;
    mx[dir][mid] = a[mid];
    for (unsigned int i = min(vl, vr, cmp); vl <= i && i <= vr && cmp(i, mid); i = nxt(i, 1))
    {
        const real nc = cost(i, mid);
        if (nc > mx[dir][mid])
        {
            pos = i;
            mx[dir][mid] = nc;
        }
        else if (nc == mx[dir][mid])
            pos = i;
    }
    conquer<dir>(pl, mid - 1, vl, pos);
    conquer<dir>(mid + 1, pr, pos, vr);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    conquer<true>(1, n, 1, n);
    conquer<false>(1, n, 1, n);
    for (unsigned int i = 1; i <= n; ++i)
        cout << max<unsigned int>(ceil(mx[0][i]), ceil(mx[1][i])) - a[i] << "\n";
    return 0;
}