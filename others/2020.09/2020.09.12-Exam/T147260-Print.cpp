#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 500;

unsigned long long ans1 = 0, ans2;
unsigned int n;
unsigned int vr[maxn + 10][maxn + 10], vc[maxn + 1][maxn + 1];

bool check(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 0; i < n; ++i)
        if (accumulate(vr[i], vr[i] + n, 0u) != k || accumulate(vc[i], vc[i] + n, 0u) != k)
            return false;
    return true;
}
void dfs(const unsigned int x, const unsigned int y)
{
    if (x == n)
    {
        ans1 += check(n, 1);
        ans2 += check(n, 2);
        return;
    }
    if (y == n)
    {
        dfs(x + 1, 0);
        return;
    }
    for (unsigned int i = 0; i < 3; ++i)
    {
        vr[x][y] = vc[y][x] = i;
        dfs(x, y + 1);
    }
}
int main()
{
    while (true)
    {
        cout << "T3 print>" << flush;
        cin >> n;
        if (cin.fail())
            break;
        ans1 = ans2 = 0;
        dfs(0, 0);
        cout << ans1 << " " << ans2 << "\n";
    }
    return 0;
}