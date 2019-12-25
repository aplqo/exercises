#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
typedef int resType;
typedef int datType;
const int maxn = 30005, maxm = 30;

datType v[maxm], w[maxm], n, m;
resType ans[maxn];
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= m; ++i)
    {
        cin >> v[i] >> w[i];
        w[i] *= v[i];
    }
    for (datType oi = m; oi > 0; oi--)
        for (datType vi = n; vi >= v[oi]; --vi)
            ans[vi] = max(ans[vi], ans[vi - v[oi]] + w[oi]);
    cout << ans[n] << endl;
    return 0;
}