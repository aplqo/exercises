#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
const unsigned int maxn = 300, maxm = 10;
const unsigned int maxv = maxn * maxm * 2 + 2, maxe = (maxn * maxm * maxm * 2 + maxn * maxn * maxm + maxv) * 2;
const unsigned int inf = UINT_MAX;

struct Edge
{
    unsigned int from, to;
    int cost;
    mutable unsigned int flow;
    union
    {
        const Edge* rev;
        unsigned int id;
    };
};
template <class T, class Pred>
class Heap
{
public:
    void clear() { ptr = dat; }
    template <class... Args>
    void emplace(Args&&... args)
    {
        *(ptr++) = T(args...);
        std::push_heap(dat, ptr, Pred());
    }
    inline const T& top() const { return dat[0]; }
    void pop()
    {
        std::pop_heap(dat, ptr, Pred());
        --ptr;
    }
    inline bool empty() const { return ptr == dat; }

private:
    T dat[maxe + 10], *ptr;
};
struct
{
    std::vector<Edge> edge;
    unsigned int h;
} vertex[maxv + 10];
const Edge *cur[maxv + 10], *head[maxv + 10];
unsigned int dis[maxv + 10];
unsigned int l[maxn][maxm], r[maxn][maxm];
unsigned int offset, source, sink, n, m;

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int flow, const int cost)
{
    vertex[from].edge.push_back(Edge { from, to, cost, flow, { .id = vertex[to].edge.size() } });
    vertex[to].edge.push_back(Edge { to, from, -cost, 0, { .id = vertex[from].edge.size() - 1 } });
}
bool dijkstra(const unsigned int s, const unsigned int t, const unsigned int n)
{
    typedef std::pair<int, unsigned int> Pair;
    static bool vis[maxv + 10];
    static Heap<Pair, std::greater<Pair>> q;
    q.clear();
    std::memset(vis, 0, sizeof(bool) * n);
    std::memset(dis, 0xff, sizeof(unsigned int) * n);
    q.emplace(0, s);
    dis[s] = 0;
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (const Edge& i : vertex[cur.second].edge)
            if (i.flow && !vis[i.to])
            {
                const unsigned int nxt = cur.first + vertex[i.from].h + i.cost - vertex[i.to].h;
                if (dis[i.to] > nxt)
                {
                    dis[i.to] = nxt;
                    q.emplace(dis[i.to], i.to);
                }
            }
    }
    return vis[t];
}
int augment(const unsigned int x, const unsigned int flow, const unsigned int sink)
{
    static bool vis[maxv + 10];
    if (x == sink || !flow)
        return flow;
    vis[x] = true;
    unsigned int rst = flow;
    for (const Edge*& i = cur[x]; i <= &vertex[x].edge.back() && rst; ++i)
        if (i->flow && !vis[i->to] && dis[x] + vertex[i->from].h + i->cost - vertex[i->to].h == dis[i->to])
        {
            const unsigned int nxt = augment(i->to, std::min(rst, i->flow), sink);
            i->flow -= nxt;
            i->rev->flow += nxt;
            rst -= nxt;
        }
    vis[x] = false;
    return flow - rst;
}
std::pair<unsigned int, unsigned int> ssp(const unsigned int n, const unsigned int source, const unsigned int sink)
{
    int mnc = 0, mxf = 0;
    while (dijkstra(source, sink, n))
    {
        std::memcpy(cur, head, sizeof(const Edge*) * n);
        const int t = augment(source, inf, sink);
        for (unsigned int i = 0; i < n; ++i)
            if (dis[i] != inf)
                vertex[i].h += dis[i];
        mxf += t;
        mnc += vertex[sink].h * t;
    }
    return std::make_pair(mxf, mnc);
}

inline unsigned int id(const unsigned int desk, const unsigned int seat)
{
    return desk * m + seat;
}
static void init(const unsigned int n, const unsigned int m)
{
    offset = n * m;
    source = offset * 2;
    sink = source + 1;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < m; ++j)
        {
            const unsigned int cid = id(i, j) + offset;
            addEdge(cid, j + 1 < m ? cid + 1 : id(i, 0) + offset, inf, 1);
            addEdge(cid, j ? cid - 1 : id(i, m - 1) + offset, inf, 1);
            addEdge(cid, sink, 1, 0);
        }
}
void addPeople(const unsigned int desk, const unsigned int seat, const unsigned int l, const unsigned int r)
{
    const unsigned int cid = id(desk, seat);
    addEdge(source, cid, 1, 0);
    for (unsigned int i = l; i <= r; ++i)
        addEdge(cid, id(i, seat) + offset, 1, i > desk ? (i - desk) * 2 : (desk - i) * 2);
}
void readMatrix(const unsigned int n, const unsigned int m, unsigned int dest[][maxm])
{
    for (unsigned int i = 0; i < n; ++i)
        std::copy_n(std::istream_iterator<unsigned int>(std::cin), m, dest[i]);
}
static void buildGraph(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < m; ++j)
            addPeople(i, j, l[i][j], r[i][j]);
}
static void buildEdge(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        for (Edge& j : vertex[i].edge)
            j.rev = &vertex[j.to].edge[j.id];
        head[i] = &vertex[i].edge.front();
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cin >> n >> m;
    init(n, m);
    readMatrix(n, m, l);
    readMatrix(n, m, r);
    buildGraph(n, m);
    buildEdge(sink + 1);
    const auto [flw, cst] = ssp(sink + 1, source, sink);
    if (flw != n * m)
        std::cout << "no solution\n";
    else
        std::cout << cst << "\n";
    return 0;
}