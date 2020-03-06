#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
using num_t = int;
const int maxn = 100;

num_t mat[maxn + 10][maxn + 10];
bool pat[maxn + 10][maxn + 10];
num_t rem[maxn + 10];
unsigned int vis[maxn + 10];

inline bool bfs(const unsigned int n, const num_t v)
{
    static unsigned int stamp = 0;
    queue<unsigned int> q;
    ++stamp;
    rem[0] = v;
    q.push(0);
    while (!q.empty())
    {
        unsigned int cur = q.front();
        q.pop();
        for (unsigned int i = 1; i <= n; ++i)
        {
            if (!pat[cur][i])
                continue;
            if (vis[i] != stamp)
            {
                vis[i] = stamp;
                rem[i] = rem[cur] * 2 + mat[cur][i];
                if (rem[i])
                    q.push(i);
            }
            else if (rem[i] < rem[cur] * 2 + mat[cur][i])
            {
                rem[i] = rem[cur] * 2 + mat[cur][i];
                if (rem[i] > 0)
                    q.push(i);
            }
        }
    }
}