#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 250000;

unsigned int stk[maxn + 1], *top = stk;

int main()
{
    unsigned int n, ans;
    cin >> n;
    ans = n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int t, c;
        cin >> t >> c;
        while (*top > c)
            --top;
        if (*top == c)
            --ans;
        *(++top) = c;
    }
    cout << ans << endl;
    return 0;
}