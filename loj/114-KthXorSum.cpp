#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxl = 50, maxn = 1e5;

class LinearBasis
{
public:
    void build(const unsigned long long* b, const unsigned long long* e)
    {
        unsigned long long tmp[maxl + 1] = {};
        for (const unsigned long long* i = b; i < e; ++i)
            insert(tmp, *i);
        for (unsigned int i = 0; i <= maxl; ++i)
            if (tmp[i])
                *(cur++) = tmp[i];
    }
    unsigned long long kth(unsigned long long k) const
    {
        k -= zero;
        unsigned long long ret = 0;
        for (int i = size() - 1; i >= 0; --i)
            if (k & (1ull << i))
                ret ^= a[i];
        return ret;
    }
    unsigned int size() const
    {
        return cur - a;
    }

private:
    void insert(unsigned long long tp[], unsigned long long x)
    {
        for (int i = maxl; i >= 0 && x; --i)
        {
            if (!(x & (1ull << i)))
                continue;
            if (tp[i])
                x ^= tp[i];
            else
            {
                for (unsigned int j = 0; j < i; ++j)
                    if (x & (1ull << j))
                        x ^= tp[j];
                for (unsigned int j = i + 1; j <= maxl; ++j)
                    if (tp[j] & (1ull << i))
                        tp[j] ^= x;
                tp[i] = x;
                break;
            }
        }
        if (!x)
            zero = true;
    }

    bool zero;
    unsigned long long a[maxl + 1], *cur = a;
} lb;
unsigned long long num[maxn + 1];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    copy_n(istream_iterator<unsigned long long>(cin), n, num);
    lb.build(num, num + n);
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned long long k;
        cin >> k;
        if (k >= (1ull << lb.size()))
            cout << "-1" << endl;
        else
            cout << lb.kth(k) << endl;
    }
    return 0;
}