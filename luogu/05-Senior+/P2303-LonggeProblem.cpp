#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdint>
#include <iostream>
#include <utility>
using namespace std;
using num_t = uint_fast64_t;
using pair_t = pair<num_t, num_t>;
constexpr num_t maxn = num_t(1) << 31, maxq = num_t(1) << 15;

pair_t fact[maxq + 10], *fc = fact;

inline void factor(const num_t n)
{
    num_t c = n;
    for (num_t i = 2; i * i <= n && c >= 1; ++i)
        if (!(c % i))
        {
            fc->first = i;
            for (; !(c % i); c /= i)
                ++(fc->second);
            ++fc;
        }
    if (c > 1)
        *(fc++) = make_pair(c, 1);
}

num_t dfs(const pair_t* pos, const num_t n)
{
    static num_t phi = 1, d = 1;
    if (pos == fc)
        return phi * (n / d);
    const auto tes = [&pos, &n](const num_t dp, const num_t dd) -> num_t {
        phi *= dp;
        d *= dd;
        num_t ret = dfs(pos + 1, n);
        d /= dd;
        phi /= dp;
        return ret;
    };
    num_t ret = tes(1, 1), dp = pos->first - 1, dd = pos->first;
    for (num_t i = 1; i <= pos->second; ++i)
    {
        ret += tes(dp, dd);
        dp *= pos->first;
        dd *= pos->first;
    }
    return ret;
}

int main()
{
    ios_base::sync_with_stdio(false);
    num_t n;
    cin >> n;
    factor(n);
    cout << dfs(fact, n) << endl;
    return 0;
}