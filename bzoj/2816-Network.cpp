#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
using namespace std;
const unsigned int maxn = 1e4, maxc = 10, maxm = 1e5;

enum Result
{
    Success = 0,
    NotExist = 1,
    Cycle = 2,
    Degree = 3,
    NoPath
};
const char* errStr[] = {
    "Success.", "No such edge.", "Error 2.", "Error 1."
};
class LinkCutTree
{
public:
    void build(const unsigned int n, const unsigned int val[]);
    template <bool check>
    bool link(const unsigned int x, const unsigned int y);
    void cut(const unsigned int x, const unsigned int y);
    pair<Result, unsigned int> minPath(const unsigned int x, const unsigned int y);
    void modify(const unsigned int x, const unsigned int val);

private:
    struct SplayNode
    {
        inline void pushRev();
        inline void pushdown();
        inline void pushup();
        inline bool isRoot();
        void pushPath();
        void rotate();
        void splay();

        bool rev = false;
        unsigned int val, maxVal = 0;
        SplayNode *fa = nullptr, *chl[2] = {};
    } snodes[maxn + 10];

    void access(SplayNode* x);
    void makeRoot(SplayNode* x);
    void split(SplayNode* const x, SplayNode* const rt);
    SplayNode* findRoot(SplayNode* x);
};
LinkCutTree lct[maxc + 1];
map<unsigned int, unsigned int> tree[maxn + 10];
unsigned int degree[maxn + 10][maxc + 10];
unsigned int val[maxn + 10];
unsigned int c;

void LinkCutTree::build(const unsigned int n, const unsigned int val[])
{
    for (unsigned int i = 1; i <= n; ++i)
        snodes[i].val = snodes[i].maxVal = val[i];
}
template <bool check>
bool LinkCutTree::link(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *const py = snodes + y;
    makeRoot(px);
    if (check)
    {
        if (findRoot(py) == px)
            return false;
    }
    px->fa = py;
    return true;
}
void LinkCutTree::cut(const unsigned int x, const unsigned int y)
{
    SplayNode *const px = snodes + x, *py = snodes + y;
    split(px, py);
    px->fa = py->chl[0] = nullptr;
    py->pushup();
}
pair<Result, unsigned int> LinkCutTree::minPath(const unsigned int x, const unsigned int y)
{
    if (x == y)
        return make_pair(Success, snodes[x].val);
    SplayNode *const px = snodes + x, *const py = snodes + y;
    if (findRoot(py) != findRoot(px))
        return make_pair(NoPath, 0);
    split(px, py);
    return make_pair(Success, py->maxVal);
}
void LinkCutTree::modify(const unsigned int x, const unsigned int val)
{
    SplayNode* const px = snodes + x;
    px->splay();
    px->val = val;
    px->pushup();
}
void LinkCutTree::access(SplayNode* x)
{
    for (SplayNode* lst = nullptr; x; x = x->fa)
    {
        x->splay();
        x->chl[1] = lst;
        x->pushup();
        lst = x;
    }
}
void LinkCutTree::makeRoot(SplayNode* x)
{
    access(x);
    x->splay();
    x->pushRev();
}
void LinkCutTree::split(SplayNode* x, SplayNode* rt)
{
    makeRoot(x);
    access(rt);
    rt->splay();
}
LinkCutTree::SplayNode* LinkCutTree::findRoot(SplayNode* x)
{
    access(x);
    x->splay();
    while (x->chl[0])
        x = x->chl[0];
    x->splay();
    return x;
}
inline void LinkCutTree::SplayNode::pushRev()
{
    swap(chl[0], chl[1]);
    rev ^= 1;
}
inline void LinkCutTree::SplayNode::pushdown()
{
    if (!rev)
        return;
    for (auto i : chl)
        if (i)
            i->pushRev();
    rev = false;
}
inline void LinkCutTree::SplayNode::pushup()
{
    maxVal = val;
    for (auto i : chl)
        maxVal = i ? max(maxVal, i->maxVal) : maxVal;
}
inline bool LinkCutTree::SplayNode::isRoot()
{
    return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
}
void LinkCutTree::SplayNode::pushPath()
{
    if (!isRoot())
        fa->pushPath();
    pushdown();
}
void LinkCutTree::SplayNode::rotate()
{
    SplayNode *const ff = fa->fa, *const f = fa;
    if (!f->isRoot())
        ff->chl[ff->chl[1] == f] = this;
    const bool typ = f->chl[1] == this;
    f->fa = this;
    fa = ff;
    if (chl[!typ])
        chl[!typ]->fa = f;
    f->chl[typ] = chl[!typ];
    chl[!typ] = f;
    f->pushup();
    this->pushup();
}
void LinkCutTree::SplayNode::splay()
{
    pushPath();
    while (!isRoot())
    {
        SplayNode* const ff = fa->fa;
        if (!fa->isRoot())
        {
            if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
                this->rotate();
            else
                fa->rotate();
        }
        this->rotate();
    }
}

inline void addEdge(unsigned int x, unsigned int y, const unsigned int col)
{
    if (x > y)
        swap(x, y);
    tree[x][y] = col;
    ++degree[x][col];
    ++degree[y][col];
    if (!lct[col].link<false>(x, y))
        throw 0;
}
Result changeColor(unsigned int u, unsigned int v, const unsigned int col)
{
    if (u > v)
        swap(u, v);
    const auto it = tree[u].find(v);
    if (it == tree[u].end())
        return NotExist;
    if (it->second == col)
        return Success;
    if (degree[u][col] + 1 > 2 || degree[v][col] + 1 > 2)
        return Degree;
    if (!lct[col].link<true>(u, v))
        return Cycle;

    lct[it->second].cut(u, v);
    --degree[u][it->second];
    --degree[v][it->second];

    ++degree[u][col];
    ++degree[v][col];

    it->second = col;
    return Success;
}
void changeWeight(const unsigned int x, const unsigned int val)
{
    for (LinkCutTree* i = lct; i < lct + c; ++i)
        i->modify(x, val);
}
static void init(const unsigned int n)
{
    for (LinkCutTree* i = lct; i < lct + c; ++i)
        i->build(n, val);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k;
    cin >> n >> m >> c >> k;
    copy_n(istream_iterator<unsigned int>(cin), n, val + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, c;
        cin >> u >> v >> c;
        addEdge(u, v, c);
    }
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int op, v1, v2, v3;
        cin >> op >> v1 >> v2;
        switch (op)
        {
        case 0:
            changeWeight(v1, v2);
            break;
        case 1:
            cin >> v3;
            cout << errStr[changeColor(v1, v2, v3)] << "\n";
            break;
        case 2:
            cin >> v3;
            const auto res = lct[v1].minPath(v2, v3);
            if (res.first == NoPath)
                cout << "-1";
            else
                cout << res.second;
            cout.put('\n');
            break;
        }
    }
    return 0;
}