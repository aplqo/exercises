#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
typedef long long num_t;

num_t exgcd(num_t a, num_t b, num_t& x, num_t& y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    num_t g = exgcd(b, a % b, x, y);
    num_t t = x;
    x = y;
    y = t - (a / b) * y;
    return g;
}

int main()
{
    ios_base::sync_with_stdio(false);
    num_t x, y, n, m, l;
    cin >> x >> y >> m >> n >> l;
    num_t anst, tmp, g = exgcd(l, n - m, tmp, anst);
    if ((x - y) % g)
    {
        cout << "Impossible" << endl;
        return 0;
    }
    anst *= (x - y) / g;
    g = g < 0 ? -g : g;
    if (anst < 0)
        anst += (-anst + l / g) / (l / g) * (l / g);
    anst %= l / g;
    cout << anst << endl;
    return 0;
}