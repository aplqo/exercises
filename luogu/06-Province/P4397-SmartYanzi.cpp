#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxq = 50000;
const unsigned long long maxs = 2e9, maxl = 34;

unsigned int primes[maxq + 10], mindiv[maxq + 10], *pcur = primes;
unsigned long long sum[maxq + 10][maxl + 1], po[maxq + 10][maxl + 1];
vector<unsigned long long> ans;

static void eular(const unsigned int n)
{
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j * i <= n && *j <= mindiv[i]; ++j)
            mindiv[*j * i] = *j;
    }
}
static void initPow()
{
    const unsigned int pcnt = pcur - primes;
    for (unsigned int i = 0; i < pcnt; ++i)
    {
        unsigned long long acc = primes[i] * primes[i];
        po[i][0] = sum[i][0] = 1;
        for (unsigned int j = 1;; ++j)
        {
            sum[i][j] = (acc - 1) / (primes[i] - 1);
            po[i][j] = po[i][j - 1] * primes[i];
            if (sum[i][j] > maxs * 2)
                break;
            acc *= primes[i];
        }
    }
}
static bool isPrime(const unsigned long long x)
{
    for (const unsigned int* i = primes; i < pcur && *i < x; ++i)
        if (x % *i == 0)
            return false;
    return true;
}
template <bool chk>
void dfs(const unsigned int pos, const unsigned long long s, const unsigned long long acc = 1)
{
    if (s == 1)
    {
        ans.push_back(acc);
        return;
    }
    if constexpr (chk)
    {
        if (s > primes[pos] && isPrime(s - 1))
            ans.push_back(acc * (s - 1));
    }
    if (po[pos][2] > s)
        return;
    dfs<false>(pos + 1, s, acc);
    for (unsigned int i = 1; sum[pos][i] <= s; ++i)
        if (s % sum[pos][i] == 0)
            dfs<true>(pos + 1, s / sum[pos][i], acc * po[pos][i]);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    eular(maxq);
    initPow();
    while (cin)
    {
        unsigned long long s;
        cin >> s;
        if (cin.fail())
            break;
        dfs<true>(0, s);
        sort(ans.begin(), ans.end());
        cout << ans.size() << "\n";
        if (ans.size())
        {
            copy(ans.cbegin(), ans.cend(), ostream_iterator<unsigned long long>(cout, " "));
            cout.put('\n');
        }
        ans.clear();
    }
    return 0;
}