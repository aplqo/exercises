#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
const int maxn = 5e5 + 5;
typedef unsigned int num;
#define lowbit(x) ((x) & -(x))

num tree[maxn];
int n;

void update(int x, num val)
{
    for (int i = x; i <= n; i += lowbit(i))
        tree[i] += val;
}
num sum(int x)
{
    if (x == 0)
        return 0;
    num ret = 0;
    for (; x; x -= lowbit(x))
        ret += tree[x];
    return ret;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int m;
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        int v;
        cin >> v;
        update(i, v);
    }
    for (int i = 0; i < m; ++i)
    {
        char op;
        int x;
        cin >> op >> x;
        switch (op)
        {
        case '1':
            num k;
            cin >> k;
            update(x, k);
            break;
        case '2':
            int y;
            cin >> y;
            cout << sum(y) - sum(x - 1) << endl;
            break;
        }
    }
    return 0;
}