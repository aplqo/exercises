#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const int maxn = 20;

unsigned int h[maxn + 10], b, n;
unsigned int ans = UINT_MAX;

void fun(unsigned int beg, unsigned int cur)
{
    if (cur >= b)
    {
        ans = min(ans, cur - b);
        return;
    }
    for (unsigned int i = beg; i < n; ++i)
        fun(i + 1, cur + h[i]);
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> b;
    for (unsigned int* i = h; i < h + n; ++i)
        cin >> *i;
    fun(0, 0);
    cout << ans << endl;
    return 0;
}