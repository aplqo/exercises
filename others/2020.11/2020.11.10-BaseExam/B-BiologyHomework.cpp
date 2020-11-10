#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;

enum class Ord
{
    Less,
    Equal,
    Greater
} trans[maxn + 10];

bool check(const unsigned int n)
{
    if (n == 1)
        return trans[1] == Ord::Equal;
    unsigned int lst = 0;
    const bool existEq = any_of(trans + 1, trans + 1 + n, [](Ord a) { return a == Ord::Equal; });
    bool existGe = false;
    for (const auto* i = trans + 1; i < trans + 1 + n; ++i)
        if (*i != Ord::Less)
        {
            if (existEq && existGe && i - trans - lst <= 2)
                return true;
            existGe = true;
            lst = i - trans;
        }
    return false;
}
void transRead(const unsigned int n, const unsigned int k)
{
    for (auto* i = trans + 1; i < trans + 1 + n; ++i)
    {
        unsigned int v;
        cin >> v;
        if (v < k)
            *i = Ord::Less;
        else if (v == k)
            *i = Ord::Equal;
        else
            *i = Ord::Greater;
    }
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
        unsigned int n, k;
        cin >> n >> k;
        transRead(n, k);
        cout << (check(n) ? "yes" : "no") << "\n";
    }
    return 0;
}