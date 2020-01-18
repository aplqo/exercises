#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const int maxn = 100010;

int h[maxn], n;

int fun(int lst, bool d) // d= true: up false: down
{
    int ret = 0;
    for (int* i = h; i < h + n; ++i)
    {
        if ((d ^ (*i < lst)) && (*i != lst))
        {
            ++ret;
            d = !d;
        }
        lst = *i;
    }
    return ret;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    for (int* i = h; i < h + n; ++i)
        cin >> *i;
    cout << max(fun(INT_MIN, true), fun(INT_MAX, false)) << endl;
    return 0;
}