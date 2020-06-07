#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
using namespace std;
const int maxn = 500;
const int maxe = maxn, maxv = maxn * 2;

struct vertex;
struct edge
{
    vertex* to;
    edge* pre = nullptr;
} ed[maxe * 2], *cur = ed;
struct vertex
{
    unsigned int dfn = 0, low = 0;
    bool isCut = false;
    edge* head = nullptr;

    void addEdge(const unsigned int to);
} ve[maxv + 10];
void vertex::addEdge(const unsigned int to)
{
    cur->to = ve + to;
    cur->pre = head;
    head = cur;
    ++cur;
}
struct bcc
{
    unsigned int cnt = 0;
    bool cut = false, ign = false;

    void addPoint(const vertex* x)
    {
        if (x->isCut)
        {
            ign = cut;
            cut = true;
        }
        else
            ++cnt;
    }
} bc[maxe + 10], *ben = bc;

void init()
{
    memset(ve, 0, sizeof(ve));
    memset(bc, 0, sizeof(bc));
    ben = bc;
    cur = ed;
}
void tarjan(vertex* const root, vertex* const x)
{
    static unsigned int sta = 0, top = 0;
    static vertex* stk[maxv + 10];
    bcc* tmp[maxv + 10];
    unsigned int cnt = 0, tTop = 0;
    x->dfn = x->low = ++sta;
    stk[++top] = x;
    for (edge* i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            ++cnt;
            tarjan(root, to);
            x->low = min(x->low, to->low);
            if ((x != root && x->dfn <= to->low) || (x == root && cnt > 1))
                x->isCut = true;
            if (x->dfn <= to->low)
            {
                do
                {
                    ben->addPoint(stk[top--]);
                } while (stk[top + 1] != to);
                tmp[tTop++] = ben++;
            }
        }
        else
            x->low = min(x->low, to->dfn);
    }
    for (unsigned int i = 0; i < tTop; ++i)
        tmp[i]->addPoint(x);
}
pair<unsigned int, unsigned long long> calc()
{
    unsigned long long sol = 1;
    unsigned int ans = 0;
    for (bcc* i = bc; i < ben; ++i)
    {
        if (i->ign)
            continue;
        if (i->cut)
        {
            ++ans;
            sol *= i->cnt;
        }
        else
        {
            ans += 2;
            sol *= i->cnt * (i->cnt - 1) / 2;
        }
    }
    return make_pair(ans, sol);
}

void solve(const unsigned int id)
{
    unsigned int n = 0, mv = 0;
    cin >> n;
    if (!n)
        exit(0);
    init();
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int s, t;
        cin >> s >> t;
        ve[s].addEdge(t);
        ve[t].addEdge(s);
        mv = max({ mv, s, t });
    }
    for (unsigned int i = 1; i <= mv; ++i)
        if (!ve[i].dfn)
            tarjan(ve + i, ve + i);
    auto ans = calc();
    cout << "Case " << id << ": " << ans.first << " " << ans.second << endl;
}
int main()
{
    for (unsigned int i = 1;; ++i)
    {
        solve(i);
        memset(ve, 0, sizeof(ve));
        memset(bc, 0, sizeof(bc));
        cur = ed;
        ben = bc;
    }
    return 0;
}