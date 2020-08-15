#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxl = 50;

class LinearBasis
{
public:
    void insert(unsigned long long x)
    {
        for (int i = maxl; i >= 0 && x; --i)
        {
            if (!(x & (1ull << i)))
                continue;
            if (a[i])
                x ^= a[i];
            else
            {
                for (unsigned int j = 0; j < i; ++j)
                    if (x & (1ull << j))
                        x ^= a[j];
                for (unsigned int j = i + 1; j <= maxl; ++j)
                    if (a[j] & (1ull << i))
                        a[j] ^= x;
                a[i] = x;
                break;
            }
        }
    }
    unsigned long long max() const
    {
        unsigned long long ret = 0;
        for (unsigned int i = 0; i <= maxl; ++i)
            ret ^= a[i];
        return ret;
    }

private:
    unsigned long long a[maxl + 1] = {};
} lb;
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned long long v;
        cin >> v;
        lb.insert(v);
    }
    cout << lb.max() << endl;
    return 0;
}