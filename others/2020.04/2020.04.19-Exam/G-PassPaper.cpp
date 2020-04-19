/*Luogu team T129704: pass paper*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
using namespace std;
using hash_t = uint_fast64_t;
constexpr hash_t b = 97;
const int maxl = 100;

char str[maxl + 10];
size_t len;
hash_t po[maxl + 10] = { 1 }, has[maxl + 10];

void GetPow()
{
    for (unsigned int i = 1; i <= len; ++i)
        po[i] = po[i - 1] * b;
}
void init()
{
    for (unsigned int i = 1; i <= len; ++i)
        has[i] = has[i - 1] * b + str[i];
}
hash_t GetHash(const unsigned int l, const unsigned int r)
{
    return has[r] - has[l - 1] * po[r - l + 1];
}

bool test(const unsigned int x)
{
    set<hash_t> v;
    for (unsigned int i = 1; i + x - 1 <= len; ++i)
    {
        hash_t h = GetHash(i, i + x - 1);
        if (v.find(h) == v.end())
            v.insert(h);
        else
            return false;
    }
    return true;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> (str + 1);
    len = strlen(str + 1);
    GetPow();
    init();
    for (unsigned int i = 0; i <= len; ++i)
        if (test(i + 1))
        {
            cout << i << endl;
            return 0;
        }
    return 0;
}