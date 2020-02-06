#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

int exgcd(int a, int b, int& x, int& y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    int g = exgcd(b, a % b, x, y), t;
    t = x;
    x = y;
    y = t - (a / b) * y;
    return g;
}
inline void solve(int a, int b, int d)
{
    int x, y, g = a > b ? exgcd(a, b, x, y) : exgcd(b, a, y, x);
    int dx = b / g, dy = -a / g;
    auto test = [&x, &y](int nx, int ny) {
        if (abs(x) + abs(y) > abs(nx) + abs(ny))
        {
            x = nx;
            y = ny;
        }
    };
    x *= d / g;
    y *= d / g;

    test(x - (x / dx) * dx, y - (x / dx) * dy);
    test(x - (y / dy) * dx, y - (y / dy) * dy);
    test(x - dx, y - dy);
    test(x + dx, y + dy);
    cout << abs(x) << " " << abs(y);
}
int main()
{
    ios_base::sync_with_stdio(false);
    bool f = false;
    while (true)
    {
        int a, b, d;
        cin >> a >> b >> d;
        if (a == 0 && b == 0 && d == 0)
            return 0;
        if (f)
            cout << endl;
        solve(a, b, d);
        f = true;
    }
    return 0;
}