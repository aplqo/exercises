#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 2e5;
using heap = priority_queue<unsigned int>;

vector<unsigned int> chl[maxn + 1];
unsigned int siz[maxn + 1];
unsigned int mem[maxn + 1];
heap h[maxn + 1];

void dfsSize(const unsigned int x)
{
    siz[x] = 1;
    for (unsigned int i : chl[x])
    {
        dfsSize(i);
        siz[x] += siz[i];
    }
    sort(chl[x].begin(), chl[x].end(), [](unsigned int a, unsigned int b) { return siz[a] < siz[b]; });
}
void merge(heap& cur, heap& other)
{
    static unsigned int tmp[maxn + 1];
    unsigned int* back = tmp;
    while (!cur.empty() && !other.empty())
    {
        *(back++) = max(cur.top(), other.top());
        cur.pop();
        other.pop();
    }
    if (cur.empty())
        swap(cur, other);
    for (unsigned int* i = tmp; i < back; ++i)
        cur.push(*i);
}
void dfs(const unsigned int x)
{
    if (chl[x].empty())
    {
        h[x].push(mem[x]);
        return;
    }
    for (unsigned int i : chl[x])
    {
        dfs(i);
        merge(h[x], h[i]);
    }
    h[x].push(mem[x]);
}
unsigned long long sum(heap& h)
{
    unsigned long long ret = 0;
    while (!h.empty())
    {
        ret += h.top();
        h.pop();
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, mem + 1);
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int f;
        cin >> f;
        chl[f].push_back(i);
    }
    dfsSize(1);
    dfs(1);
    cout << sum(h[1]) << endl;
    return 0;
}