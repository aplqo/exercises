#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
using namespace std;
const unsigned int maxp = 1e5;
const long long inf = LLONG_MAX / 2;
typedef map<pair<unsigned int, unsigned int>, unsigned int> PointMap;

struct Edge
{
    unsigned int from;
    const Edge* pre;
} ed[maxp * 2 + 10];
struct Point
{
    Point()
        : head(NULL)
        , dis(inf) {};
    long long dis;
    long long x, y;
    const Edge* head;
} po[maxp * 2 + 10];
unsigned int cnt, dcnt;
PointMap mp;
Point* seq[maxp * 2 + 10];
long long orig[maxp * 2 + 10];
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur));
    long long query(const unsigned int r);
    void add(const long long delt);
    void update(const unsigned int p, const long long val);

private:
    inline void push(const long long l);
    inline void pushdown();
    unsigned int lv, mv, rv;
    STree *lc, *rc;
    long long val, laz;
} Snodes[maxp * 2 * 4 + 100], *root = Snodes;
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    val = laz = 0;
    if (r - l == 1)
        return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
    val = min(lc->val, rc->val);
}
long long STree::query(const unsigned int r)
{
    if (rv <= r)
        return val;
    pushdown();
    if (mv < r)
        return min(lc->query(mv), rc->query(r));
    else
        return lc->query(r);
}
void STree::add(const long long delt)
{
    push(delt);
}
void STree::update(const unsigned int p, const long long v)
{
    if (rv - lv == 1)
    {
        val = v;
        return;
    }
    pushdown();
    if (p < mv)
        lc->update(p, v);
    else
        rc->update(p, v);
    val = min(lc->val, rc->val);
}
inline void STree::push(const long long l)
{
    laz += l;
    val += l;
}
inline void STree::pushdown()
{
    if (!laz)
        return;
    lc->push(laz);
    rc->push(laz);
    laz = 0;
}

inline unsigned int findPoint(const unsigned int x, const unsigned int y)
{
    const pair<unsigned int, unsigned int> p(x, y);
    PointMap::iterator it = mp.find(p);
    if (it != mp.end())
        return it->second;
    po[cnt].x = x;
    po[cnt].y = y;
    return mp[p] = cnt++;
}
inline void addEdge(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
    static Edge* cur = ed;
    const unsigned int a = findPoint(x1, y1), b = findPoint(x2, y2);
    cur->from = a;
    cur->pre = po[b].head;
    po[b].head = cur;
    ++cur;
}
static void discretize(const unsigned int n)
{
    long long* cur = orig;
    for (Point* i = po; i < po + n; ++i)
        *(cur++) = i->y;
    sort(orig, cur);
    cur = unique(orig, cur);
    dcnt = cur - orig;
    for (Point* i = po; i < po + n; ++i)
        i->y = lower_bound(orig, cur, i->y) - orig;
}
inline bool cmpPtr(const Point* a, const Point* b)
{
    return a->x != b->x ? a->x < b->x : a->y < b->y;
}
static void init()
{
    discretize(cnt);
    for (unsigned int i = 0; i < cnt; ++i)
        seq[i] = po + i;
    sort(seq, seq + cnt, cmpPtr);
    {
        STree* c = Snodes;
        root->build(0, dcnt, c);
    }
}
long long solve(const unsigned int n)
{
    for (unsigned int i = 1; i < cnt; ++i)
    {
        Point* const cp = seq[i];
        if (cp->x != seq[i - 1]->x)
            root->add(cp->x - seq[i - 1]->x);
        cp->dis = root->query(cp->y + 1) + orig[cp->y];
        for (const Edge* i = cp->head; i; i = i->pre)
            cp->dis = min(cp->dis, po[i->from].dis);
        root->update(cp->y, cp->dis - orig[cp->y]);
    }
    return po[findPoint(n, n)].dis;
}

int main()
{
#if !defined APTEST && !defined APDEBUG
    ios_base::sync_with_stdio(false);
    ifstream in("boards.in");
    ofstream out("boards.out");
#else
    istream& in = cin;
    ostream& out = cout;
#endif
    unsigned int n, p;
    in >> n >> p;
    {
        cnt = 2;
        po[0].x = po[0].y = 0;
        po[0].dis = 0;
        po[1].x = po[1].y = n;
        mp[make_pair(0, 0)] = 0;
        mp[make_pair(n, n)] = 1;
    }
    for (unsigned int i = 0; i < p; ++i)
    {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
            continue;
        addEdge(x1, y1, x2, y2);
    }
    init();
    out << solve(n) << endl;
    return 0;
}