#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
using namespace std;
const unsigned int maxn = 2000;
using real = long double;

real p[maxn + 1];
real ans;
bool sel[maxn + 1];
unsigned int n, k;

real dp(const unsigned int n)
{
    real f[maxn + 1];
    fill(f + 1, f + n + 1, 0);
    f[0] = 1;
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!sel[i])
            continue;
        ++cnt;
        for (unsigned int j = cnt; j > 0; --j)
            f[j] = f[j] * (1 - p[i]) + f[j - 1] * p[i];
        f[0] *= 1 - p[i];
    }
    return f[cnt / 2];
}
void dfs(const unsigned int pos, const unsigned int val)
{
    if (pos == n || !val)
    {
        if (!val)
            ans = max(ans, dp(n));
        return;
    }
    dfs(pos + 1, val);
    sel[pos] = true;
    dfs(pos + 1, val - 1);
    sel[pos] = false;
}

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream fans(argv[2]);
    in >> n >> k;
    copy_n(istream_iterator<real>(in), n, p);
    dfs(0, k);
    fans << fixed << ans << "\n";
    return 0;
}