#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 4e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int a[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
namespace LCT
{
    struct Node
    {
        void rotate();
        void splay();
        bool isRoot() const;
        void pushup();

        unsigned long long realSize, virtSize, val, answer;
        Node *fa, *chl[2];
    } nodes[maxn + 100];

    void Node::pushup()
    {
        realSize = val + virtSize;
        for (auto i : chl)
            if (i)
                realSize += i->realSize;
    }
    void Node::rotate()
    {
        Node *const f = fa, *const ff = fa->fa;
        if (!f->isRoot())
            ff->chl[ff->chl[1] == f] = this;
        const bool typ = f->chl[1] == this;
        fa = ff;
        f->fa = this;
        if (chl[!typ])
            chl[!typ]->fa = f;
        f->chl[typ] = chl[!typ];
        chl[!typ] = f;
        f->pushup();
        this->pushup();
    }
    void Node::splay()
    {
        while (!isRoot())
        {
            Node* const ff = fa->fa;
            if (!fa->isRoot())
            {
                if ((ff->chl[1] == fa) ^ (fa->chl[1] == this))
                    this->rotate();
                else
                    fa->rotate();
            }
            rotate();
        }
    }
    bool Node::isRoot() const
    {
        return fa ? fa->chl[0] != this && fa->chl[1] != this : true;
    }

    unsigned long long build(const unsigned int x, const unsigned int f)
    {
        unsigned long long ans = 0;
        Node* const ptr = nodes + x;
        ptr->val = ptr->realSize = a[x];
        unsigned long long ms = a[x];
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            ans += build(to, x);
            nodes[to].fa = ptr;
            ptr->realSize += nodes[to].realSize;
            if (nodes[to].realSize > ms)
            {
                ms = nodes[to].realSize;
                ptr->chl[1] = &nodes[to];
            }
        }
        if (ms * 2 <= ptr->realSize)
            ptr->chl[1] = nullptr;
        ptr->virtSize = (ptr->chl[1] ? ptr->realSize - ms : ptr->realSize) - ptr->val;
        ptr->answer = std::min(ptr->realSize - 1, (ptr->realSize - ms) * 2);
        return ans + ptr->answer;
    }
    unsigned long long add(const unsigned int x, const unsigned int w)
    {
        long long delt = 0;
        for (Node *ptr = nodes + x, *lst = nullptr; ptr; ptr = ptr->fa)
        {
            ptr->splay();
            ptr->realSize += w;
            if (ptr == nodes + x)
                ptr->val += w;
            const unsigned long long size = ptr->realSize - (ptr->chl[0] ? ptr->chl[0]->realSize : 0);
            delt -= ptr->answer;
            if (ptr->chl[1] && ptr->chl[1]->realSize * 2 > size)
            {
                if (lst)
                    ptr->virtSize += w;
                ptr->answer = (size - ptr->chl[1]->realSize) * 2;
            }
            else
            {
                if (ptr->chl[1])
                {
                    ptr->virtSize += ptr->chl[1]->realSize;
                    ptr->chl[1] = nullptr;
                }
                if (lst && lst->realSize * 2 > size)
                {
                    ptr->chl[1] = lst;
                    ptr->virtSize -= lst->realSize - w;
                    ptr->answer = (size - lst->realSize) * 2;
                }
                else
                {
                    if (lst)
                        ptr->virtSize += w;
                    ptr->answer = ptr->val * 2 > size ? (size - ptr->val) * 2 : size - 1;
                }
            }
            delt += ptr->answer;
            lst = ptr;
        }
        return delt;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
    unsigned int n, m;
    std::cin >> n >> m;
    std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, a + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        std::cin >> u >> v;
        addEdge(u, v);
    }
    unsigned long long ans = LCT::build(1, 0);
    std::cout << ans << "\n";
    for (unsigned int i = m; i; --i)
    {
        unsigned int x, w;
        std::cin >> x >> w;
        ans += LCT::add(x, w);
        std::cout << ans << "\n";
    }
    return 0;
}