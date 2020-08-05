#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
using num_t = long long;
const int maxn = 200000;

struct position
{
    void remove()
    {
        if (pre)
            pre->nxt = nxt;
        if (nxt)
            nxt->pre = pre;
    }
    position *pre = nullptr, *nxt = nullptr;
    unsigned int sta = 0;
    num_t val;
} p[maxn + 10];
class heap
{
public:
    void init(position* l, position* r)
    {
        for (position* i = l; i < r; ++i)
            dat.push(make_pair(i, 0));
    }
    position* top()
    {
        clean();
        return dat.top().first;
    }
    void push(position* i)
    {
        clean();
        dat.push(make_pair(i, i->sta));
    }
    void pop()
    {
        clean();
        dat.pop();
        clean();
    }
    void remove(position* i)
    {
        clean();
        del.push(make_pair(i, i->sta));
        clean();
    }

private:
    void clean()
    {
        while (!dat.empty() && !del.empty() && dat.top() == del.top())
        {
            dat.pop();
            del.pop();
        }
    }
    using datType = pair<position*, unsigned int>;
    struct cmp
    {
        bool operator()(datType l, datType r)
        {
            return l.first->val != r.first->val ? l.first->val > r.first->val : l.first < r.first;
        }
    };
    using pq = priority_queue<datType, vector<datType>, cmp>;
    pq dat, del;
} h;

void init(const unsigned int n)
{
    for (position* i = p + 1; i < p + n; ++i)
    {
        i->pre = i - 1;
        i->nxt = i + 1;
    }
    p[1].pre = nullptr;
    p[n - 1].nxt = nullptr;
    h.init(p + 1, p + n);
}
num_t solve(const unsigned int m)
{
    num_t ret = 0;
    for (unsigned int i = 0; i < m; ++i)
    {
        position* const t = h.top();
        h.pop();
        ret += t->val;
        t->val = (t->pre ? t->pre->val : 0) + (t->nxt ? t->nxt->val : 0) - t->val;
        const bool fl = !t->pre || !t->nxt;
        if (t->pre)
        {
            h.remove(t->pre);
            t->pre->remove();
        }
        if (t->nxt)
        {
            h.remove(t->nxt);
            t->nxt->remove();
        }
        if (fl)
        {
            t->remove();
            continue;
        }
        ++(t->sta);
        h.push(t);
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    num_t lst = 0;
    cin >> lst;
    for (unsigned int i = 1; i < n; ++i)
    {
        num_t c;
        cin >> c;
        p[i].val = c - lst;
        lst = c;
    }
    init(n);
    cout << solve(m) << endl;
    return 0;
}