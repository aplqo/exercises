#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 5e4;

struct Object
{
    unsigned int p, q;
} obj[maxn + 10];

unsigned int greedy(const unsigned int n, const unsigned int k, unsigned long long m)
{
    priority_queue<unsigned int, vector<unsigned int>, greater<unsigned int>> heap;
    sort(obj, obj + n, [](const Object& a, const Object& b) { return a.q < b.q; });
    unsigned int ret = 0;
    for (const Object* i = obj; i < obj + k; ++i)
    {
        if (m < i->q)
            break;
        m -= i->q;
        heap.push(i->p - i->q);
        ++ret;
    }
    if (ret < k || !m)
        return ret;
    sort(obj + k, obj + n, [](const Object& a, const Object& b) { return a.p < b.p; });
    for (const Object* i = obj + k; i < obj + n && m; ++i)
    {
        const unsigned int v1 = i->p, v2 = heap.top() + i->q;
        if (v1 > m && v2 > m)
            continue;
        m -= min(v1, v2);
        if (v2 < v1)
            heap.pop();
        ++ret;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    unsigned long long m;
    cin >> n >> k >> m;
    for (Object* i = obj; i < obj + n; ++i)
        cin >> i->p >> i->q;
    cout << greedy(n, k, m) << "\n";
    return 0;
}