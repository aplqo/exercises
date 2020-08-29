#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;
const unsigned int maxn = 300;

unsigned int a[maxn * 2 + 1], cnt[maxn * 2 + 1][maxn * 2 + 1];

void countDiff(const unsigned int n)
{
    static bool exist[maxn + 1];
    unsigned int val = 0;
    for (unsigned int l = 1; l <= n; ++l)
    {
        for (unsigned int r = l; r <= n; ++r)
        {
            if (!exist[a[r]])
            {
                ++val;
                exist[a[r]] = true;
            }
            cnt[l][r] = val;
        }
        for (unsigned int* p = a + l; p < a + n + 1; ++p)
            exist[*p] = false;
        val = 0;
    }
}
unsigned long long dp(const unsigned int n, const unsigned int len)
{
    static unsigned long long f[maxn * 2 + 10][maxn * 2 + 10];
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int l = 1; l + i - 1 <= n; ++l)
        {
            const unsigned int r = l + i - 1;
            for (unsigned int k = l; k < r; ++k)
                f[l][r] = max(f[l][r], f[l][k] + f[k + 1][r] + cnt[l][k] * cnt[k + 1][r]);
        }
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ret = max(ret, f[i][i + len - 1]);
    return ret;
}
int main()
{
#if !defined APTEST && !defined APDEBUG
    ios_base::sync_with_stdio(false);
    ifstream in("merge.in");
    ofstream out("merge.out");
#else
    istream& in = cin;
    ostream& out = cout;
#endif
    unsigned int n;
    in >> n;
    for (unsigned int* i = a + 1; i < a + n + 1; ++i)
        in >> *i;
    copy(a + 1, a + 1 + n, a + 1 + n);
    countDiff(n * 2);
    out << dp(n * 2, n) << "\n";
    return 0;
}