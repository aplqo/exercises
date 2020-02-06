#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;
using num_t = unsigned long long;
constexpr unsigned int npr = 15;
constexpr num_t inf = numeric_limits<num_t>::max();
constexpr num_t primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47 };

num_t ans = 0, n, num = inf;

void dfs(size_t pos, num_t cur, unsigned int lst)
{
    static num_t t = 1;
    num_t bas = primes[pos];
    if (t > ans || (t == ans && cur <= num))
    {
        ans = t;
        num = cur;
    }
    if (bas * cur > n || pos >= npr)
        return;
    for (unsigned int i = 1; i <= lst && cur * bas <= n; ++i, bas *= primes[pos])
    {
        t *= i + 1;
        dfs(pos + 1, cur * bas, i);
        t /= i + 1;
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    while (cin)
    {
        cin >> n;
        if (cin.fail())
            return 0;
        dfs(0, 1, ceil(log2l(n)) + 1);
        cout << num << endl;
        ans = 0;
        num = inf;
    }
    return 0;
}