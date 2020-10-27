#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 3e4;

unsigned int ch[maxn + 10][2], fail[maxn + 10];
bool b[maxn + 10];
enum status
{
    NUL = 0,
    STK = 1,
    VIS = 2
} vis[maxn + 10];

inline void add(const char str[], size_t len)
{
    static unsigned int cur = 0;
    unsigned int k = 0;
    for (size_t i = 0; i < len; ++i)
    {
        int c = str[i] - '0';
        if (!ch[k][c])
            ch[k][c] = ++cur;
        k = ch[k][c];
    }
    b[k] = true;
}
void make()
{
    queue<unsigned int> q;
    for (unsigned int i = 0; i < 2; ++i)
        if (ch[0][i])
            q.push(ch[0][i]);
    while (!q.empty())
    {
        unsigned int cur = q.front();
        q.pop();
        for (unsigned int i = 0; i < 2; ++i)
            if (!ch[cur][i])
                ch[cur][i] = ch[fail[cur]][i];
            else
            {
                q.push(ch[cur][i]);
                fail[ch[cur][i]] = ch[fail[cur]][i];
            }
    }
}
bool dfs(unsigned int x)
{
    vis[x] = STK;
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (vis[ch[x][i]] == VIS)
            continue;
        bool fl = true;
        for (unsigned int j = x; j; j = fail[j])
            if (b[ch[j][i]])
            {
                fl = false;
                break;
            }
        if (fl && (vis[ch[x][i]] == STK || dfs(ch[x][i])))
            return true;
    }
    vis[x] = VIS;
    return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    unsigned int n;
    static char str[maxn + 10];
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> str;
        add(str, strlen(str));
    }
    make();
    cout << (dfs(0) ? "TAK" : "NIE") << endl;
    return 0;
}