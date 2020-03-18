#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <iostream>
#define toInt(x) (isupper(x) ? ((x) - 'A') : ((x) - 'a' + 26))
using namespace std;
const unsigned int maxc = 26 * 2, maxn = maxc * maxc;

bool adj[maxc + 10][maxc + 10];
unsigned int degree[maxc + 10];
unsigned int seq[maxn + 10], *pos = seq;

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    unsigned int ifrom = toInt(from);
    unsigned int ito = toInt(to);
    adj[ifrom][ito] = true;
    adj[ito][ifrom] = true;
    ++degree[ifrom];
    ++degree[ito];
}
void dfs(const unsigned int x)
{
    for (unsigned int i = 0; i < maxc; ++i)
    {
        if (!adj[i][x])
            continue;
        adj[i][x] = adj[x][i] = false;
        dfs(i);
    }
    *(pos++) = x;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        char a, b;
        cin >> a >> b;
        AddEdge(a, b);
    }

    const auto isOdd = [](const unsigned int i) { return i % 2; };
    const unsigned int odd = count_if(degree, degree + maxc, isOdd);
    unsigned int beg;
    if (odd > 2)
    {
        cout << "No Solution" << endl;
        return 0;
    }
    else if (odd)
        beg = find_if(degree, degree + maxc, isOdd) - degree;
    else
        beg = find_if(degree, degree + maxc, [](const unsigned int i) -> bool { return i; }) - degree;
    dfs(beg);
    if (pos - seq > n + 1)
    {
        cout << "No Solution" << endl;
        return 0;
    }
    reverse(seq, pos);
    for (unsigned int* i = seq; i < pos; ++i)
    {
        char c = *i > 25 ? (*i - 26 + 'a') : (*i + 'A');
        cout << c;
    }
    cout << endl;
    return 0;
}