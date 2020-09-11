#include <algorithm>
#include <fstream>
#include <numeric>
using namespace std;
const unsigned int maxn = 30;

bool able[maxn + 1][maxn + 1];
bool sel[maxn + 1], ans[maxn + 1];
unsigned int seq[maxn + 1], n;

void dfs(const unsigned int x)
{
    if (x == n)
    {
        for (unsigned int i = 0; i < n; ++i)
            ans[i] = ans[i] || !sel[i];
        return;
    }
    bool found = false;
    for (unsigned int i = 0; i < n; ++i)
        if (able[seq[x]][i] && !sel[i])
        {
            sel[i] = true;
            found = true;
            dfs(x + 1);
            sel[i] = false;
        }
    if (!found)
        dfs(x + 1);
}
unsigned int count(const unsigned int n)
{
    iota(seq, seq + n, 0);
    do
    {
        dfs(0);
    } while (next_permutation(seq, seq + n));
    return count(ans, ans + n, true);
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    in >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        static char str[maxn + 1];
        in >> str;
        for (unsigned int j = 0; j < n; ++j)
            able[i][j] = str[j] == '1';
    }
    ans << count(n) << "\n";
    return 0;
}