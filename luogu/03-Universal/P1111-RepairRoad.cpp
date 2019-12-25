#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1005, maxm = 100005;
typedef unsigned int datType;

struct road
{
    datType x, y;
    int t;
} ro[maxm];
datType fa[maxn], cnt[maxn];
datType GetFather(datType x)
{
    return fa[x] == x ? x : (fa[x] = GetFather(fa[x]));
}
int main()
{
    ios_base::sync_with_stdio(false);
    datType n, m;
    cin >> n >> m;
    for (road* i = ro; i < ro + m; ++i)
        cin >> i->x >> i->y >> i->t;
    sort(ro, ro + m, [](const road& a, const road& b) -> bool { return a.t <= b.t; });
    fill(cnt, cnt + 1 + n, 1);
    iota(fa + 1, fa + 1 + n, 1);
    for (road* i = ro; i < ro + m; ++i)
    {
        datType f1 = GetFather(i->x), f2 = GetFather(i->y);
        if (f1 != f2)
        {
            fa[f2] = f1;
            cnt[f1] += cnt[f2];
        }
        if (cnt[f1] == n)
        {
            cout << i->t << endl;
            return 0;
        }
    }
    cout << "-1" << endl;
    return 0;
}
