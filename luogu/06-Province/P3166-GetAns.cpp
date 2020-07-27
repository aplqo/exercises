#include <fstream>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000;

unsigned long long ans;
unsigned int n, m;
long long p[3][2];

bool test()
{
    return (p[1][0] - p[0][0]) * (p[2][1] - p[0][1]) - (p[2][0] - p[0][0]) * (p[1][1] - p[0][1]) != 0;
}
void dfs(const unsigned int dep, const unsigned int limx, const unsigned int limy)
{
    if (dep == 3)
    {
        ans += test();
        return;
    }
    for (unsigned int x = limx; x <= n; ++x)
    {
        p[dep][0] = x;
        for (unsigned int y = x == limx ? limy : 0; y <= m; ++y)
        {
            p[dep][1] = y;
            dfs(dep + 1, x, y);
        }
    }
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream fans(argv[2]);
    in >> n >> m;
    dfs(0, 0, 0);
    fans << ans << endl;
    return 0;
}