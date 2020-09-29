#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const unsigned int maxn = 500, maxm = 500, maxl = 1000, maxq = 1000;
typedef bitset<maxl> Vector;

struct Edge
{
    unsigned int to;
    const Edge *pre, *rev;
    Vector val;
} ed[(maxm * 2) * 2 + 10];
const Edge* head[maxn + 10];
struct Train
{
    unsigned int from, to;
    unsigned int beg;
    bool vis;
    Vector val;
} tr[maxq + 10];
class LinearBasis
{
public:
    void insert(Vector v);
    Vector max() const;

private:
    Vector val[maxl];
};
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur));
    void add(const unsigned int l, const unsigned int r, const Vector& va);
    void dfs(LinearBasis lb) const;

private:
    STree *lc, *rc;
    unsigned int lv, mv, rv;
    vector<Vector> val;
} nodes[maxq * 4 + 10], *root = nodes;
LinearBasis init;
unsigned int n;
Vector dis[maxn + 10], ans[maxq + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const Vector& val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from], cur + 1, val };
    head[from] = cur++;
    new (cur) Edge { from, head[to], cur - 1, val };
    head[to] = cur++;
}
Vector readVector(istream& is)
{
    Vector ret;
    string str;
    is >> str;
    for (unsigned int i = 0; i < str.size(); ++i)
        if (str[i] == '1')
            ret.set(str.size() - i - 1);
    return ret;
}
void writeVector(ostream& os, const Vector& v)
{
    unsigned int pos = maxl - 1;
    while (pos && !v.test(pos))
        --pos;
    if (!pos)
    {
        os << "0";
        return;
    }
    for (int i = pos; i >= 0; --i)
        os << (v.test(i) ? '1' : '0');
}

void LinearBasis::insert(Vector v)
{
    for (int i = maxl - 1; i >= 0; --i)
    {
        if (!v.test(i))
            continue;
        if (val[i].count())
            v ^= val[i];
        else
        {
            for (unsigned int j = 0; j < i; ++j)
                if (v.test(j))
                    v ^= val[j];
            for (unsigned int j = i + 1; j < maxl; ++j)
                if (val[j].test(i))
                    val[j] ^= v;
            val[i] = v;
            break;
        }
    }
}
Vector LinearBasis::max() const
{
    Vector ret;
    for (int i = maxl - 1; i >= 0; --i)
        ret ^= val[i];
    return ret;
}
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    if (r - l == 1)
        return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
}
void STree::add(const unsigned int l, const unsigned int r, const Vector& v)
{
    if (l == lv && r == rv)
    {
        val.emplace_back(v);
        return;
    }
    if (r <= mv)
        lc->add(l, r, v);
    else if (l >= mv)
        rc->add(l, r, v);
    else
    {
        lc->add(l, mv, v);
        rc->add(mv, r, v);
    }
}
void STree::dfs(LinearBasis lb) const
{
    for (const Vector& i : val)
        lb.insert(i);
    if (rv - lv == 1)
        ans[lv] = lb.max();
    else
    {
        lc->dfs(lb);
        rc->dfs(lb);
    }
}

void dfs(const unsigned int x, const Edge* re)
{
    static bool vis[maxn + 10];
    static unsigned int dep[maxn + 10];
    vis[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        if (i == re)
            continue;
        if (!vis[i->to])
        {
            dis[i->to] = dis[x] ^ i->val;
            dep[i->to] = dep[x] + 1;
            dfs(i->to, i->rev);
        }
        else if (dep[i->to] <= dep[x])
            init.insert(dis[i->to] ^ dis[x] ^ i->val);
    }
}

inline unsigned int readIndex(istream& is) // read and minus 1
{
    unsigned int ret;
    is >> ret;
    return ret - 1;
}
inline void readEdge(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u = readIndex(cin), v = readIndex(cin);
        const Vector ve = readVector(cin);
        addEdge(u, v, ve);
    }
}
inline void readQuery(const unsigned int n, const unsigned int q)
{
    static const auto cancel = [](Train& t, const unsigned int cur) {
        root->add(t.beg, cur, dis[t.from] ^ dis[t.to] ^ t.val);
        t.vis = false;
    };
    static const auto update = [](Train& t, const unsigned int cur) {
        t.val = readVector(cin);
        t.beg = cur;
        t.vis = true;
    };
    unsigned int sta = 0;
    for (unsigned int i = 0; i < q; ++i)
    {
        char buf[10];
        cin >> buf;
        switch (buf[1])
        {
        case 'd': // Add
        {
            tr[sta].from = readIndex(cin);
            tr[sta].to = readIndex(cin);
            update(tr[sta++], i);
            break;
        }
        case 'a': // Cancel
            cancel(tr[readIndex(cin)], i);
            break;
        case 'h': // Change
        {
            Train& t = tr[readIndex(cin)];
            cancel(t, i);
            update(t, i);
            break;
        }
        }
    }
    for (const Train* i = tr; i < tr + sta; ++i)
        if (i->vis)
            root->add(i->beg, q, dis[i->from] ^ dis[i->to] ^ i->val);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m, p;
    cin >> n >> m >> p;
    readEdge(n, m);
    {
        STree* c = nodes;
        root->build(0, p + 1, c);
        dfs(0, nullptr);
    }
    readQuery(n, p);
    root->dfs(init);
    writeVector(cout, init.max());
    cout << "\n";
    for (unsigned int i = 0; i < p; ++i)
    {
        writeVector(cout, ans[i]);
        cout << "\n";
    }
    return 0;
}