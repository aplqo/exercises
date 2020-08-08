#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
constexpr unsigned int inf = INT_MAX / 2;
const unsigned int maxn = 1000, maxp = 1e5;

struct Point
{
    Point() = default;
    Point(const unsigned int x, const unsigned int y)
        : x(x)
        , y(y) {};
    unsigned int x, y;

    template <class T>
    inline T& operator[](T d[][maxn + 1]) const { return d[x][y]; }
    inline bool operator<(const Point& r) const { return x != r.x ? x < r.x : y < r.y; }
};

struct Edge
{
    Point to;
    const Edge* pre;
} ed[maxp + 1];
const Edge* head[maxn + 1][maxn + 1];
unsigned long long dis[maxn + 1][maxn + 1];
bool vis[maxn + 1][maxn + 1];

void addEdge(const Point from, const Point to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = from[head];
    from[head] = cur;
    ++cur;
}
unsigned long long dijkstra(const unsigned int n)
{
    using pair_t = pair<unsigned long long, Point>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    for (unsigned int i = 0; i <= n; ++i)
        fill(dis[i], dis[i] + 1 + n, inf);
    const Point o(0, 0);
    o[dis] = 0;
    q.emplace(0, o);
    const auto update = [&q](const Point to, const unsigned int d) {
        if (to[dis] > d)
        {
            to[dis] = d;
            q.emplace(to[dis], to);
        }
    };
    while (!q.empty())
    {
        const pair_t cur = q.top();
        q.pop();
        const Point np = cur.second;
        if (np[vis])
            continue;
        np[vis] = true;
        for (const Edge* i = np[head]; i; i = i->pre)
            update(i->to, cur.first);
        update(Point(np.x, np.y + 1), cur.first + 1);
        update(Point(np.x + 1, np.y), cur.first + 1);
    }
    return Point(n, n)[dis];
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, p;
    in >> n >> p;
    for (unsigned int i = 0; i < p; ++i)
    {
        unsigned int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        addEdge(Point(x1, y1), Point(x2, y2));
    }
    ans << dijkstra(n) << endl;
    return 0;
}