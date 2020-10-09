#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <bitset>
#include <climits>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 30000;
constexpr unsigned int inf = UINT_MAX / 2;

struct State
{
    unsigned int pos, step;
    inline bool operator==(const State& r) const { return pos == r.pos && step == r.step; }
};
typedef deque<pair<State, unsigned int>> Queue;
bitset<maxn + 10> vis[maxn + 10];
vector<unsigned int> doges[maxn + 10];
unsigned int b[maxn + 10], p[maxn + 10];

template <unsigned int dis>
void update(Queue& q, const State st, const unsigned int val)
{
    if (vis[st.pos].test(st.step))
        return;
    vis[st.pos].set(st.step);
    if constexpr (dis > 0)
        q.emplace_back(st, val);
    else
        q.emplace_front(st, val);
}
unsigned int bfs(const unsigned int n)
{
    Queue q;
    update<0>(q, State { b[0], p[0] }, 0);
    while (!q.empty())
    {
        const auto [cur, val] = q.front();
        if (cur.pos == b[1])
            return val;
        q.pop_front();
        if (!doges[cur.pos].empty())
        {
            for (unsigned int i : doges[cur.pos])
                if (i != cur.step)
                    update<0>(q, State { cur.pos, i }, val);
            doges[cur.pos].clear();
        }
        if (cur.pos >= cur.step)
            update<1>(q, State { cur.pos - cur.step, cur.step }, val + 1);
        if (cur.pos + cur.step <= n)
            update<1>(q, State { cur.pos + cur.step, cur.step }, val + 1);
    }
    return inf;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        cin >> b[i] >> p[i];
        doges[b[i]].push_back(p[i]);
    }
    const unsigned int v = bfs(n);
    if (v == inf)
        cout << "-1\n";
    else
        cout << v << "\n";
    return 0;
}