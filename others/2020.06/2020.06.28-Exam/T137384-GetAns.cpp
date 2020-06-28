#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1000;

unsigned int val[maxn + 1][maxn + 2];
bool reach[maxn + 1][maxn + 2];
unsigned long long f[maxn + 1][maxn + 2];

unsigned long long dp(const unsigned int n, const unsigned int x, const unsigned int y)
{
    memset(f, 0, sizeof(f));
    memset(reach, 0, sizeof(reach));
    f[1][1] = val[1][1];
    reach[1][1] = true;
    for (unsigned int i = 1; i < n; ++i)
    {
        for (unsigned int j = 1; j <= i; ++j)
        {
            if ((x == i && y == j) || !reach[i][j])
            {
                reach[i][j] = false;
                continue;
            }
            f[i + 1][j] = max(f[i + 1][j], f[i][j] + val[i + 1][j]);
            reach[i + 1][j] = true;
            f[i + 1][j + 1] = max(f[i + 1][j + 1], f[i][j] + val[i + 1][j + 1]);
            reach[i + 1][j + 1] = true;
        }
    }
    f[x][y] = 0;
    return *max_element(f[n] + 1, f[n] + 1 + n);
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, q;
    in >> n >> q;
    for (unsigned int i = 1; i <= n; ++i)
        copy_n(istream_iterator<unsigned int>(in), i, val[i] + 1);
    for (unsigned int j = 0; j < q; ++j)
    {
        unsigned int x, y;
        in >> x >> y;
        if (x == 1 && y == 1)
            ans << "-1" << endl;
        else
            ans << dp(n, x, y) << endl;
    }
    return 0;
}
