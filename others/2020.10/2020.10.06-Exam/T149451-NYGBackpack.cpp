#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

struct Object
{
    unsigned int a, b;
} obj[maxn + 10];

bool greedy(const unsigned int n, unsigned long long v)
{
    Object* p = partition(obj, obj + n, [](const Object& a) { return a.a < a.b; });
    sort(obj, p, [](const Object& a, const Object& b) { return a.a < b.a; });
    sort(p, obj + n, [](const Object& a, const Object& b) { return a.b > b.b; });
    for (const Object* i = obj; i < obj + n; ++i)
    {
        if (v < i->a)
            return false;
        v = v - i->a + i->b;
    }
    return true;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned int n, v;
        cin >> n >> v;
        for (Object* i = obj; i < obj + n; ++i)
            cin >> i->a >> i->b;
        cout << (greedy(n, v) ? "Yes\n" : "No\n");
    }
    return 0;
}