#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const int maxn = 6000;

struct peo
{
    void AddChild(peo& c)
    {
        chl.push_back(&c);
        c.root = false;
    }
    void dp()
    {
        for (auto i : chl)
            i->dp();
        for (auto i : chl)
        {
            f[false] += max(i->f[true], i->f[false]);
            f[true] += i->f[false];
        }
        f[true] += h;
    }

    int h, f[2] = {};
    vector<peo*> chl;
    bool root = true;
} p[maxn + 10];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
        cin >> p[i].h;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int l, k;
        cin >> l >> k;
        p[k].AddChild(p[l]);
    }
    peo* r = find_if(p + 1, p + 1 + n, [](const peo& i) { return i.root; });
    r->dp();
    cout << max(r->f[false], r->f[true]) << endl;
    return 0;
}