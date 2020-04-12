#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <limits>
#include <numeric>
#include <utility>
using namespace std;
using num_t = unsigned int;
constexpr num_t inf = numeric_limits<num_t>::max();
const int maxn = 30, maxm = 5000;

unsigned int sol[maxn + 10][maxm + 10];
num_t f[maxn + 10][maxm + 10];
struct child
{
    num_t g;
    unsigned int cnt = 0;
} chl[maxn + 10];
child* seq[maxn + 10];
num_t sum[maxn + 10];

void init(const unsigned int n, const unsigned int m)
{
    transform(chl, chl + 1 + n, seq, [](child& i) { return &i; });
    sort(seq + 1, seq + 1 + n, [](const child* a, const child* b) { return a->g > b->g; });
    for (unsigned int i = 1; i <= n; ++i)
        sum[i] = sum[i - 1] + seq[i]->g;
    for (unsigned int i = 0; i <= n; ++i)
        fill(f[i], f[i] + 1 + m, inf);
    f[0][0] = 0;
}
void dp(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = i; j <= m; ++j)
        {
            auto& sc = sol[i][j];
            if (j > i)
                f[i][j] = f[i][j - i];
            for (unsigned int k = 1; k <= i; ++k)
            {
                const num_t val = f[i - k][j - k] + (i - k) * (sum[i] - sum[i - k]);
                if (val < f[i][j])
                {
                    f[i][j] = val;
                    sc = k;
                }
            }
        }
}
void getAnswer(const unsigned int i, const unsigned int j)
{
    if (!i || !j)
        return;
    const auto cur = sol[i][j];
    if (cur)
    {
        for (unsigned int k = 0; k < cur; ++k)
            seq[i - k]->cnt = 1;
        getAnswer(i - cur, j - cur);
    }
    else
    {
        getAnswer(i, j - i);
        for (unsigned int k = 1; k <= i; ++k)
            ++(seq[k]->cnt);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    ifstream in("cookies.in");
    ofstream out("cookies.out");
    unsigned int n, m;
    in >> n >> m;
    for (child* i = chl + 1; i < chl + 1 + n; ++i)
        in >> i->g;
    init(n, m);
    dp(n, m);
    getAnswer(n, m);
    out << f[n][m] << endl;
    for (unsigned int i = 1; i <= n; ++i)
        out << chl[i].cnt << " ";
    return 0;
}