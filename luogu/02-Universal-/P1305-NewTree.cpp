#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
const int maxn = 28;
#define toint(c) (c == '*' ? 0 : c - 'a' + 1)

int l[maxn], r[maxn];

void print(int x)
{
    cout << char(x + 'a' - 1);
    if (l[x])
        print(l[x]);
    if (r[x])
        print(r[x]);
}
int main()
{
    ios_base::sync_with_stdio(false);
    int n, ro;
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        char c[4];
        cin >> c;
        if (!i)
            ro = toint(c[0]);
        l[toint(c[0])] = toint(c[1]);
        r[toint(c[0])] = toint(c[2]);
    }
    print(ro);
    return 0;
}