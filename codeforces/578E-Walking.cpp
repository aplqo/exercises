#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>
const unsigned int maxn = 100000;

std::set<unsigned int> s[2];
unsigned int ans[maxn + 10];

unsigned int construct()
{
    if (s[1].size() > s[0].size() || (s[0].size() == s[1].size() && *s[1].begin() < *s[0].begin()))
        s[1].swap(s[0]);
    unsigned int typ = 1, *ptr = ans, ret = 0, pos = 0;
    while (!s[0].empty() || !s[1].empty())
    {
        auto next = s[!typ].lower_bound(pos);
        if (next == s[!typ].end()
            || (*s[!typ].begin() < *s[typ].begin()
                && s[typ].lower_bound(*next) == s[typ].end()
                && *s[!typ].begin() != *next))
        {
            next = s[!typ].begin();
            ++ret;
        }
        *(ptr++) = pos = *next;
        s[!typ].erase(next);
        typ = !typ;
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    static char str[maxn + 10];
    std::cin >> (str + 1);
    const size_t n = std::strlen(str + 1);
    for (unsigned int i = 1; i <= n; ++i)
        s[str[i] == 'L'].insert(i);
    std::cout << construct() << "\n";
    std::copy(ans, ans + n, std::ostream_iterator<unsigned int>(std::cout, " "));
    return 0;
}