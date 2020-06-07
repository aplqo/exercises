#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
using namespace std;
const unsigned int maxn = 50;
const unsigned int b = 137;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * maxn + 1];
edge* head[maxn + 1];
bool vis[maxn + 1];

map<unsigned long long, unsigned int> m;

inline unsigned long long getHash(const char str[])
{
    const unsigned int l = strlen(str);
    unsigned long long ret = 0;
    for (unsigned int i = 0; i < l; ++i)
    {
        ret *= b;
        if (isupper(str[i]))
            ret += str[i] - 'A';
        else if (islower(str[i]))
            ret += str[i] - 'a' + 26;
        else
            ret += str[i] - '0' + 26 + 26;
    }
    return ret;
}
inline void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline unsigned int readVertex()
{
    char tmp[20];
    scanf("%s", tmp);
    return m[getHash(tmp)];
}

unsigned int dp(const unsigned int n)
{
    static unsigned int f[maxn + 1][maxn + 1] = {};
    f[0][0] = 1;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = i; j < n; ++j)
        {
            if (!f[i][j])
                continue;
            for (edge* e = head[j]; e; e = e->pre)
                if (e->to > j)
                    f[i][e->to] = max(f[i][e->to], f[i][j] + 1);
            for (edge* e = head[i]; e; e = e->pre)
                if (e->to > j)
                    f[j][e->to] = max(f[j][e->to], f[i][j] + 1);
        }
    unsigned int ret = 1;
    for (edge* i = head[n - 1]; i; i = i->pre)
        ret = max(ret, f[i->to][n - 1]);
    return ret;
}

int main()
{
    unsigned int n, v;
    scanf("%u%u", &n, &v);
    for (unsigned int i = 0; i < n; ++i)
    {
        char tmp[20];
        scanf("%s", tmp);
        m[getHash(tmp)] = i;
    }
    for (unsigned int i = 0; i < v; ++i)
    {
        unsigned int u, v;
        u = readVertex();
        v = readVertex();
        addEdge(u, v);
        addEdge(v, u);
    }
    printf("%d\n", dp(n));
    return 0;
}
