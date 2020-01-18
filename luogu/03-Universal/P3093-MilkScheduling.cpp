#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 10015, maxt = 20010;

struct cow
{
    int d, g;
} co[maxn];
int fre[maxt];
int ans;

int GetFree(int x)
{
    return fre[x] == x ? x : fre[x] = GetFree(fre[x]);
}

int main()
{
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    iota(fre, fre + maxt, 0);
    for (cow* i = co; i < co + n; ++i)
        cin >> i->g >> i->d;
    sort(co, co + n, [](const cow& a, const cow& b) -> bool { return a.g != b.g ? a.g > b.g : a.d > b.d; });
    for (cow* i = co; i < co + n; ++i)
    {
        int f = GetFree(i->d);
        if (f == 0)
            continue;
        fre[f] = f - 1;
        ans += i->g;
    }
    cout << ans << endl;
    return 0;
}