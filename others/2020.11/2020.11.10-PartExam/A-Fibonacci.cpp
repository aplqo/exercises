#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
using namespace std;
const unsigned int maxn = 100000;
const unsigned int fib[47] = {
    1, 1, 2, 3, 5, 8, 13, 21, 34, 55,
    89, 144, 233, 377, 610, 987, 1597, 2584,
    4181, 6765, 10946, 17711, 28657, 46368,
    75025, 121393, 196418, 317811, 514229,
    832040, 1346269, 2178309, 3524578, 5702887,
    9227465, 14930352, 24157817, 39088169,
    63245986, 102334155, 165580141, 267914296,
    433494437, 701408733, 1134903170,
    1836311903, 2971215073
};

unsigned int a[maxn + 10];

unsigned int count(const unsigned int n)
{
    unordered_set<unsigned int> st;
    unsigned int ret = 0;
    st.reserve(n);
    for (unsigned int* i = a; i < a + n; ++i)
    {
        bool fail = false;
        for (const unsigned int* f = lower_bound(fib, fib + 47, *i); !fail && f < fib + 47; ++f)
            if (st.count(*f - *i))
                fail = true;
        if (fail)
        {
            st.clear();
            st.reserve(n);
            ++ret;
        }
        st.insert(*i);
    }
    return ret + 1;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a);
    cout << count(n) << "\n";
    return 0;
}