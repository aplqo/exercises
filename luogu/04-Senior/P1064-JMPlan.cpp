#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxm = 65, maxn = 32005;

struct obj
{
    obj() = default;
    inline obj(int v, int w, int id)
        : v(v)
        , w(w * v)
        , id(id){};
    int v, w, id;
    int ans[maxn] = {};
};
struct node : obj
{
    node() = default;
    node(int v, int w, int id)
        : obj::obj(v, w, id){};

    inline void AddChild(int v, int w, int id)
    {
        child[cnt++] = obj(v, w, id);
    }
    inline void solve(int nv)
    {
        if (nv < v)
        {
            ans[nv] = pre->ans[nv];
            return;
        }
        ans[nv] = max(pre->ans[nv - v] + w, pre->ans[nv]);
        if (!cnt)
            return;
        const int cv = nv - v;
        {
            obj& c = child[0];
            c.ans[nv] = pre->ans[cv] + w;
            if (cv >= c.v)
                c.ans[nv] = max(c.ans[nv], pre->ans[cv - c.v] + w + c.w);
        }
        for (obj* oi = child + 1; oi < child + cnt; ++oi)
            if (cv >= oi->v)
                oi->ans[nv] = max((oi - 1)->ans[nv], (oi - 1)->ans[nv - oi->v] + oi->w);
            else
                oi->ans[nv] = (oi - 1)->ans[nv];
        ans[nv] = max(ans[nv], child[cnt - 1].ans[nv]);
    }
    node *pre = nullptr, *nxt = nullptr;

private:
    int cnt = 0;
    obj child[5];
} o[maxm];

int main()
{
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= m; ++i)
    {
        int v, w, p;
        cin >> v >> w >> p;
        if (p)
            o[p].AddChild(v, w, i);
        else
            o[i] = node(v, w, i);
    }
    for (node* i = o; i < o + m + 1;)
    {
        i->nxt = find_if(i + 1, o + 1 + m, [](const node& i) -> bool { return i.id; });
        i->nxt->pre = i;
        i = i->nxt;
    }
    for (int vi = 1; vi <= n; ++vi)
        for (node* oi = o[0].nxt; oi < o + 1 + m; oi = oi->nxt)
            oi->solve(vi);
    cout << o[m + 1].pre->ans[n] << endl;
    return 0;
}