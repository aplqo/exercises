#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
const int maxn = 1e6 + 5, maxm = 1e6 + 5;

int dif[maxn], thi[maxn];
int n, m;
struct sel
{
    int d, s, t;
} d[maxm];

bool test(int e)
{
    for (sel* i = d + 1; i < d + e + 1; ++i)
    {
        thi[i->s] -= i->d;
        thi[i->t + 1] += i->d;
    }
    int lst = 0;
    for (int i = 1; i <= n + 1; ++i)
    {
        lst += thi[i];
        if (lst < 0)
        {
            copy(dif, dif + 2 + n, thi);
            return false;
        }
    }
    copy(dif, dif + 2 + n, thi);
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m;
    {
        int lst = 0, thi = 0;
        for (int i = 1; i <= n; ++i)
        {
            cin >> thi;
            dif[i] = thi - lst;
            lst = thi;
        }
        dif[n + 1] = 0 - thi;
    }
    for (sel* i = d + 1; i < d + 1 + m; ++i)
        cin >> i->d >> i->s >> i->t;
    int l = 0, r = m;
    copy(dif, dif + 2 + n, thi);
    if (test(m))
    {
        cout << "0" << endl;
        return 0;
    }
    while (l <= r)
    {
        int mid = (l + r) >> 1;
        if (test(mid))
            l = mid + 1;
        else
            r = mid - 1;
    }
    cout << "-1" << endl
         << l << endl;
    return 0;
}