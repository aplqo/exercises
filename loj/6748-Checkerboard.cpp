#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
const unsigned int maxn = 300, maxm = 300;

bool select[maxn + 10][maxm + 10], full[maxm + 10];
unsigned int a[maxm + 10];

[[noreturn]] void fail()
{
    std::cout << "No\n";
    std::exit(0);
}
void constructEven(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i <= m; ++i)
        for (unsigned int j = 1 + (i & 0x01); j <= n && a[i]; j += 2)
        {
            select[j][i] = true;
            --a[i];
        }
}
void constructOdd(const unsigned int n, const unsigned int m, const unsigned int first)
{
    for (unsigned int i = 1; i <= n; i += 2)
        select[i][first] = true;
    for (unsigned int i = first - 1; i; --i)
        for (unsigned int j = ((first - i) & 0x01) + 1; a[i]; j += 2)
        {
            select[j][i] = true;
            --a[i];
        }
    unsigned int last = first;
    for (unsigned int i = first + 1; i <= m; ++i)
        if (full[i])
        {
            for (unsigned int j = 1; j <= n; j += 2)
                select[j][i] = true;
            if ((i - last) & 0x01)
            {
                for (unsigned int j = last + 1; j < i; ++j)
                {
                    for (unsigned int k = n - !((j - last) & 0x01); k && a[j] && !select[k][j - 1]; k -= 2)
                    {
                        select[k][j] = true;
                        --a[j];
                    }
                    for (unsigned int k = ((j - last) & 0x01) + 1; k <= n && a[j]; k += 2)
                    {
                        select[k][j] = true;
                        --a[j];
                    }
                    if (a[j])
                        fail();
                }
                for (unsigned int j = 1; j <= n; j += 2)
                    if (select[j][i - 1])
                        fail();
            }
            else
            {
                for (unsigned int j = last + 1; j < i; ++j)
                    for (unsigned int k = ((j - last) & 0x01) + 1; k <= n && a[j]; k += 2)
                    {
                        select[k][j] = true;
                        --a[j];
                    }
            }
            last = i;
        }
    for (unsigned int i = last + 1; i <= m; ++i)
        for (unsigned int j = 1 + ((i - last) & 0x01); j <= n && a[i]; j += 2)
        {
            select[j][i] = true;
            --a[i];
        }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, m;
    std::cin >> n >> m;
    const unsigned int lim = (n + 1) >> 1;
    unsigned int first = m + 1;
    for (unsigned int i = 1; i <= m; ++i)
    {
        std::cin >> a[i];
        if (a[i] > lim)
            fail();
        else if (a[i] == lim)
        {
            full[i] = true;
            first = std::min(first, i);
        }
    }
    if (n & 0x01)
        constructOdd(n, m, first);
    else
        constructEven(n, m);
    std::cout << "Yes\n";
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (unsigned int j = 1; j <= m; ++j)
            std::cout << select[i][j];
        std::cout.put('\n');
    }
    return 0;
}