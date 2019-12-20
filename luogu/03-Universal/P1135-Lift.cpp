#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <cstdlib>
using namespace std;
const int maxn = 205;

struct flr
{
    int ans, nxt;
    bool vis = false;
} fl[maxn];
flr *(que[maxn * 2]), **front = que, **rear = que + 1;
flr *a, *b, *n;

inline void test(flr *t, flr *fa)
{
    if (t > fl && t < n)
    {
        if (t->vis)
            return;
        t->ans = fa->ans + 1;
        t->vis = true;
        if (t == b)
        {
            cout << t->ans << endl;
            exit(0);
        }
        *rear = t;
        ++rear;
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    {
        int tn, ta, tb;
        cin >> tn >> ta >> tb;
        n = fl + tn + 1;
        a = fl + ta;
        b = fl + tb;
    }
    for (flr *i = fl + 1; i < n; ++i)
        cin >> i->nxt;
    *front = a;
    a->vis = true;
    if (a == b)
    {
        cout << "0" << endl;
        return 0;
    }
    while (front < rear)
    {
        flr *cur = *front;
        test(cur + cur->nxt, cur);
        test(cur - cur->nxt, cur);
        ++front;
    }
    cout << "-1" << endl;
    return 0;
}