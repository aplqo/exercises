#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;
const unsigned int maxn = 5000;

unsigned int x[maxn + 10];

bool check(const unsigned int col, const unsigned int n)
{
    multiset<unsigned int> st;
    for (unsigned int i = 0; i < col; ++i)
        st.insert(1);
    for (unsigned int j = col; j < n; ++j)
    {
        auto it = st.upper_bound(x[j]);
        if (it == st.begin())
            return false;
        --it;
        st.insert(*it + 1);
        st.erase(it);
    }
    return true;
}
unsigned int binarySearch(const unsigned int n)
{
    unsigned int l = 0, r = n;
    while (l <= r)
    {
        const unsigned int mid = (l + r) >> 1;
        if (check(mid, n))
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
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, x);
    sort(x, x + n);
    cout << binarySearch(n) << "\n";
    return 0;
}