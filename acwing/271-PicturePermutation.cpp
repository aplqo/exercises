#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxk = 5, maxl = 8000;

class state
{
public:
    void Init(const unsigned int n[], const unsigned int k)
    {
        this->k = k;
        off[k - 1] = 1;
        for (int i = k - 2; i >= 0; --i)
            off[i] = off[i + 1] * (n[i + 1] + 1);
        memset(dat, 0, sizeof(dat));
    }
    unsigned long long& operator[](unsigned int val[])
    {
        unsigned int pos = 0;
        for (unsigned int i = 0; i < k; ++i)
            pos += off[i] * val[i];
        return dat[pos];
    }

private:
    unsigned int off[maxk + 10], k;
    unsigned long long dat[maxl + 10];
} f;
unsigned int val[maxk + 10], n[maxk + 10];
unsigned int k;

void add(const unsigned int p)
{
    const unsigned int tmp = f[val];
    ++val[p];
    f[val] += tmp;
    --val[p];
}
void fun(const unsigned int dep)
{
    if (dep == k)
    {
        if (val[0] < n[0])
            add(0);
        for (unsigned int i = 1; i < dep; ++i)
            if (val[i] < n[i] && val[i] < val[i - 1])
                add(i);
        return;
    }
    for (unsigned int i = 0; i <= n[dep]; ++i)
    {
        val[dep] = i;
        fun(dep + 1);
    }
}
void solve()
{
    cin >> k;
    if (!k)
        exit(0);
    for (unsigned int i = 0; i < k; ++i)
        cin >> n[i];
    fill(val, val + k, 0);
    f.Init(n, k);
    f[val] = 1;
    fun(0);
    cout << f[n] << endl;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (true)
        solve();
    return 0;
}