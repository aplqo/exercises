#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <map>
#include <utility>
using namespace std;
const unsigned int maxn = 18;
constexpr unsigned long long b = 131;

char str[maxn * 2 + 10];
bool sel[maxn * 2 + 10];
map<pair<unsigned int, unsigned int>, unsigned int> mp;
unsigned int n;
unsigned long long ans;

pair<unsigned int, unsigned int> GetHash(const int l, const int r)
{
    unsigned long long h1 = 0, h2 = 0;
    for (int i = l; i < r; ++i)
        if (sel[i])
            h1 = h1 * b + str[i];
    for (int i = r - 1; i >= l; --i)
        if (!sel[i])
            h2 = h2 * b + str[i];
    return make_pair(h1, h2);
}
void dfsPre(const unsigned int pos)
{
    if (pos == n)
    {
        const auto t = GetHash(0, n);
        auto it = mp.find(t);
        if (it == mp.end())
            mp[t] = 1;
        else
            ++(it->second);
        return;
    }
    dfsPre(pos + 1);
    sel[pos] = true;
    dfsPre(pos + 1);
    sel[pos] = false;
}
void dfsSuf(const unsigned int pos)
{
    if (pos == n - 1)
    {
        auto t = GetHash(n, n * 2);
        swap(t.first, t.second);
        const auto it = mp.find(t);
        if (it != mp.end())
            ans += it->second;
        return;
    }
    dfsSuf(pos - 1);
    sel[pos] = true;
    dfsSuf(pos - 1);
    sel[pos] = false;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n >> str;
    dfsPre(0);
    dfsSuf(n * 2 - 1);
    cout << ans / 2 << "\n";
    return 0;
}