#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 100000, maxl = 18;
constexpr int inf = INT_MAX, ninf = INT_MIN / 2;

struct ValFrom
{
    unsigned int val, from;
    inline bool operator<(const ValFrom& r) const { return val < r.val; }
};
constexpr ValFrom vfInf { inf, 0 };
struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
template <class T>
class Multiset
{
public:
    inline void insert(const T x)
    {
        val.push(x);
        ++size;
    }
    inline void erase(const T x)
    {
        --size;
        if (val.top() == x)
            val.pop();
        else
            del.push(x);
        clean();
    }
    inline void change(const T x, const T y)
    {
        if (x == y)
            return;
        erase(x);
        insert(y);
    }
    inline T maxSum()
    {
        const T v1 = val.top();
        val.pop();
        clean();
        const T ret = v1 + val.top();
        val.push(v1);
        return ret;
    }
    inline T max() const { return val.top(); }
    inline bool empty() const { return size; }

    unsigned int size = 0;

private:
    inline void clean()
    {
        while (!val.empty() && !del.empty() && val.top() == del.top())
        {
            val.pop();
            del.pop();
        }
    }

    priority_queue<T> val, del;
};
struct Vertex
{
    int dist[maxl + 10];
    unsigned int centroidDep;
    Vertex* centroidFa;
    bool color = false;
    Multiset<int> distFromSub, distToFa;
    int maxPath;

    template <bool erase>
    inline void update();
} vert[maxn + 10];
Multiset<int> paths;
unsigned int siz[maxn + 10], cnt[2];
bool vis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
template <bool erase>
inline void Vertex::update()
{
    const int old = maxPath;
    if (distFromSub.size > 1)
        maxPath = distFromSub.maxSum();
    else if (!color)
        maxPath = 0;
    else
        maxPath = ninf;
    if (erase)
        paths.change(old, maxPath);
    else
        paths.insert(maxPath);
}

ValFrom dfsCentroid(const unsigned int x, const unsigned int f, const unsigned int n)
{
    unsigned int ms = 0;
    ValFrom ret = vfInf;
    siz[x] = 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f || vis[to])
            continue;
        ret = min(ret, dfsCentroid(to, x, n));
        ms = max(ms, siz[to]);
        siz[x] += siz[to];
    }
    ms = max(ms, n - siz[x]);
    return min(ret, ValFrom { ms, x });
}
void dfsDistance(const unsigned int x, const unsigned int f, Vertex& crt)
{
    const int cdist = vert[x].dist[crt.centroidDep];
    crt.distToFa.insert(vert[x].dist[crt.centroidDep - 1]);
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to != f && !vis[to])
        {
            vert[to].dist[crt.centroidDep] = cdist + 1;
            dfsDistance(to, x, crt);
        }
    }
}
void dfsCDTree(const unsigned int x)
{
    Vertex& ptr = vert[x];
    const unsigned int cid = ptr.centroidDep;
    vis[x] = true;
    ptr.dist[cid] = 0;
    dfsDistance(x, 0, vert[x]);
    ptr.distFromSub.insert(0);
    ptr.distToFa.insert(ninf);
    for (const Edge* i = head[x]; i; i = i->pre)
        if (!vis[i->to])
        {
            const unsigned int nxt = dfsCentroid(i->to, x, siz[i->to]).from;
            vert[nxt].centroidFa = vert + x;
            vert[nxt].centroidDep = cid + 1;
            dfsCDTree(nxt);
            ptr.distFromSub.insert(vert[nxt].distToFa.max());
        }
    ptr.update<false>();
}

static inline void init(const unsigned int n)
{
    const unsigned int b = dfsCentroid(1, 0, n).from;
    cnt[0] = n;
    vert[b].centroidDep = 1;
    vert[b].centroidFa = vert;
    dfsCDTree(b);
}
void change(const unsigned int x)
{
    Vertex& ptr = vert[x];
    --cnt[ptr.color];
    ptr.color = !ptr.color;
    ++cnt[ptr.color];
    if (ptr.color)
    {
        ptr.distFromSub.erase(0);
        const int* dis = ptr.dist + ptr.centroidDep - 1;
        for (Vertex* i = vert + x; i != vert;)
        {
            Vertex* const f = i->centroidFa;
            const int oldv = i->distToFa.max();
            i->distToFa.erase(*dis);
            f->distFromSub.change(oldv, i->distToFa.max());
            i->update<true>();
            i = f;
            --dis;
        }
    }
    else
    {
        ptr.distFromSub.insert(0);
        const int* dis = ptr.dist + ptr.centroidDep - 1;
        for (Vertex* i = vert + x; i != vert;)
        {
            Vertex* const f = i->centroidFa;
            const int oldv = i->distToFa.max();
            i->distToFa.insert(*dis);
            f->distFromSub.change(oldv, i->distToFa.max());
            i->update<true>();
            i = f;
            --dis;
        }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n;
    for (unsigned int i = n - 1; i; --i)
    {
        unsigned int a, d;
        cin >> a >> d;
        addEdge(a, d);
    }
    init(n);
    cin >> q;
    for (unsigned int i = q; i; --i)
    {
        char typ;
        cin >> typ;
        if (typ == 'G')
        {
            switch (cnt[0])
            {
            case 0:
                cout << "-1\n";
                break;
            case 1:
                cout << "0\n";
                break;
            default:
                cout << paths.max() << "\n";
                break;
            }
        }
        else
        {
            unsigned int x;
            cin >> x;
            change(x);
        }
    }
    return 0;
}