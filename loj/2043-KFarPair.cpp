#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <queue>
const unsigned int maxn = 1e5;

enum
{
    X = 0,
    Y = 1
};
typedef std::array<unsigned int, 2> Point;
inline unsigned long long distance(const Point& l, const Point& r)
{
    static const auto sq = [](long long x) { return x * x; };
    return sq(static_cast<long long>(l[0]) - r[0]) + sq(static_cast<long long>(l[1]) - r[1]);
}
struct Square
{
    Point low, high;

    unsigned long long maxDistance(const Point& p) const
    {
        return std::max({ distance(p, low),
            distance(p, high),
            distance(p, Point { low[X], high[Y] }),
            distance(p, Point { high[X], low[Y] }) });
    }
};
std::priority_queue<unsigned long long, std::vector<unsigned long long>, std::greater<unsigned long long>> heap;
struct Node
{
    Square space;
    Node *lc, *rc;
    bool isLeaf;

    template <unsigned int typ>
    void build(const Point** pl, const Point** pr);
    void pushup();
    void updateFar(const Point p);
} nodes[maxn * 4 + 19], root, *cur = nodes;
Point point[maxn + 10];

template <unsigned int typ>
void Node::build(const Point** pl, const Point** pr)
{
    static const auto pred = [](const Point* l, const Point* r) {
        return (*l)[typ] < (*r)[typ];
    };
    if (pr - pl == 1)
    {
        space = Square { **pl, **pl };
        isLeaf = true;
        lc = rc = nullptr;
        return;
    }
    const unsigned int len = pr - pl, mid = len >> 1;
    std::nth_element(pl, pl + mid, pr, pred);
    lc = cur++;
    lc->build<!typ>(pl, pl + mid);
    rc = cur++;
    rc->build<!typ>(pl + mid, pr);
    pushup();
}
void Node::pushup()
{
    space.low[X] = std::min(lc->space.low[X], rc->space.low[X]);
    space.high[X] = std::max(lc->space.high[X], rc->space.high[X]);
    space.low[Y] = std::min(lc->space.low[Y], rc->space.low[Y]);
    space.high[Y] = std::max(lc->space.high[Y], rc->space.high[Y]);
}
void Node::updateFar(const Point pl)
{
    if (isLeaf)
    {
        const unsigned long long d = distance(space.low, pl);
        if (d > heap.top())
        {
            heap.pop();
            heap.push(d);
        }
        return;
    }
    if (space.maxDistance(pl) <= heap.top())
        return;
    lc->updateFar(pl);
    rc->updateFar(pl);
}
void build(const unsigned int n)
{
    static const Point* p[maxn + 10];
    std::iota(p, p + n, point);
    root.build<0>(p, p + n);
}
unsigned long long solve(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 0; i < n && heap.size() < k; ++i)
        for (unsigned int j = 0; j < n && heap.size() < k; ++j)
            heap.push(distance(point[i], point[j]));
    for (unsigned int i = 0; i < n; ++i)
        root.updateFar(point[i]);
    return heap.top();
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, k;
    std::cin >> n >> k;
    for (Point* i = point; i < point + n; ++i)
        std::cin >> (*i)[X] >> (*i)[Y];
    build(n);
    std::cout << solve(n, k * 2) << "\n";
    return 0;
}