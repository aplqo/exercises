#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <climits>
#include <iostream>
using namespace std;
typedef long long num;
const int maxn = 10010;
const num inf = LLONG_MAX;

num c[maxn], y[maxn], s;
num store = inf, ans;
int n;

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> s;
    for (int i = 0; i < n; ++i)
        cin >> c[i] >> y[i];
    for (int i = 0; i < n; ++i)
    {
        if (store + i * s < c[i])
            ans += (store + i * s) * y[i];
        else
            ans += c[i] * y[i];
        if (c[i] - i * s < store)
            store = c[i] - i * s;
    }
    cout << ans << endl;
    return 0;
}
