#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
using namespace std;
typedef unsigned long long numType;
typedef pair<numType, numType> pairType;
const numType inf = numeric_limits<numType>::max();
const int maxa = 5e6 + 5, maxn = 3;

numType a[maxn], dis[maxa], l = 1, r, tar;
numType ans;
bool vis[maxa];
int n = 3;

inline void dijkstra()
{
    priority_queue<pairType, vector<pairType>, greater<pairType>> q;
    fill(dis, dis + tar + 1, inf);
    q.push(make_pair(1, 1));
    while (!q.empty())
    {
        pairType cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (numType* i = a; i < a + n; ++i)
        {
            numType pos = (cur.second + *i) % tar;
            if (dis[pos] > cur.first + *i)
            {
                dis[pos] = cur.first + *i;
                q.push(make_pair(dis[pos], pos));
            }
        }
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> r;
    for (numType* i = a; i < a + n; ++i)
    {
        cin >> *i;
        if (*i == 0)
        {
            --i;
            --n;
        }
    }
    tar = *min_element(a, a + n);
    dijkstra();
    for (numType i = 0; i < tar; ++i)
    {
        if (r < dis[i])
            continue;
        numType cl = max(l, dis[i]);
        numType ml = cl % tar, mr = r % tar;
        numType nl = (cl - ml + tar) / tar, nr = (r - mr) / tar;
        ans += nr - nl;
        if (i >= ml)
            ++ans;
        if (i <= mr)
            ++ans;
    }
    cout << ans + 1 << endl;
    return 0;
}