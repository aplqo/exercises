#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 7e4, maxm = 1.5e5;
constexpr unsigned int inf = UINT_MAX / 2;

enum
{
    X = 0,
    Y = 1
};
struct ValFrom
{
    unsigned int val, from;
};
struct ValGreater
{
    inline bool operator()(const ValFrom& l, const ValFrom& r) const
    {
        return l.val != r.val ? l.val > r.val : l.from < r.from;
    }
};
priority_queue<ValFrom, vector<ValFrom>, ValGreater> heap;
typedef array<unsigned int, 2> Point;
struct Square
{
    Point l, r;

    inline bool isInside(const Square& other) const
    {
        return other.l[X] <= l[X] && other.l[Y] <= l[Y] && r[X] <= other.r[X] && r[Y] <= other.r[Y];
    }
    inline bool isIntersect(const Square& other) const
    {
        return !(other.r[X] < l[X] || other.r[Y] < l[Y] || other.l[X] > r[X] || other.l[Y] > r[Y]);
    }
};
struct Transition
{
    Square to;
    unsigned int time;
};
unsigned int dis[maxn * 4 + 19], vcnt;
bool vis[maxn * 4 + 10];
struct City
{
    unsigned int id;
    Point point;
    vector<Transition> next;
} city[maxn + 10], *ccur = city;
namespace KDTree
{
    struct Node
    {
        unsigned int id;
        Square space;
        const Node *lc, *rc;

        template <unsigned int typ>
        void build(City** const l, City** const r);
        void update(const Square& s, const unsigned int val) const;
        inline void relaxation() const;
        inline void pushup();
    } nodes[maxn * 4 + 10], *cur = nodes, *root = nodes;
    Node* revId[maxn * 5 + 10];

    template <unsigned int typ>
    void Node::build(City** const l, City** const r)
    {
        static const auto pred = [](City* a, City* b) {
            return a->point[typ] < b->point[typ];
        };
        const unsigned int len = r - l, mid = len >> 1;
        id = ++vcnt;
        revId[id] = this;
        if (len == 1)
        {
            id = (*l)->id;
            space = Square { (*l)->point, (*l)->point };
            return;
        }
        nth_element(l, l + mid, r, pred);
        City** const pmid = l + mid;
        lc = ++cur;
        cur->build<!typ>(l, pmid);
        rc = ++cur;
        cur->build<!typ>(pmid, r);
        pushup();
    }
    inline void Node::pushup()
    {
        space.l[X] = min(lc->space.l[X], rc->space.l[X]);
        space.l[Y] = min(lc->space.l[Y], rc->space.l[Y]);
        space.r[X] = max(lc->space.r[X], rc->space.r[X]);
        space.r[Y] = max(lc->space.r[Y], rc->space.r[Y]);
    }
    void Node::update(const Square& s, const unsigned int val) const
    {
        if (!space.isIntersect(s))
            return;
        if (space.isInside(s))
        {
            if (dis[id] > val)
            {
                dis[id] = val;
                heap.push(ValFrom { dis[id], id });
            }
            return;
        }
        lc->update(s, val);
        rc->update(s, val);
    }
    inline void Node::relaxation() const
    {
        if (dis[lc->id] > dis[id])
        {
            dis[lc->id] = dis[id];
            heap.push(ValFrom { dis[lc->id], lc->id });
        }
        if (dis[rc->id] > dis[id])
        {
            dis[rc->id] = dis[id];
            heap.push(ValFrom { dis[rc->id], rc->id });
        }
    }
    inline void build(City* const l, City* const r)
    {
        static City* buf[maxn + 10];
        iota(buf, buf + (r - l), l);
        root->build<X>(buf, buf + (r - l));
    }
}

void dijkstra(const unsigned int n)
{
    fill(dis, dis + vcnt + 1, inf);
    dis[0] = 0;
    heap.push(ValFrom { .val = 0, .from = 0 });
    while (!heap.empty())
    {
        const unsigned int cur = heap.top().from;
        heap.pop();
        if (vis[cur])
            continue;
        vis[cur] = true;
        if (cur < n)
        {
            for (const auto& i : city[cur].next)
                KDTree::root->update(i.to, i.time + dis[cur]);
        }
        else
            KDTree::revId[cur]->relaxation();
    }
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    istream& is = cin;
    ostream& os = cout;
    is.tie(nullptr);
    os.tie(nullptr);
    unsigned int n, m, w, h;
    is >> n >> m >> w >> h;
    vcnt = n;
    for (City* i = city; i < city + n; ++i)
    {
        is >> i->point[X] >> i->point[Y];
        i->id = i - city;
    }
    for (unsigned int i = m; i; --i)
    {
        unsigned int p;
        Transition tp;
        is >> p >> tp.time >> tp.to.l[X] >> tp.to.r[X] >> tp.to.l[Y] >> tp.to.r[Y];
        city[p - 1].next.push_back(tp);
    }
    KDTree::build(city, city + n);
    dijkstra(n);
    copy(dis + 1, dis + n, ostream_iterator<unsigned int>(os, "\n"));
    return 0;
}