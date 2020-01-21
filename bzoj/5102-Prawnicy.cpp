#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstdlib>
using namespace std;
typedef int num;
const int maxn = 1000005;
const num inf = INT_MAX;

struct rang
{
    int id;
    num l, r;
} dat[maxn];
const rang *poi[maxn], *heap[maxn];
num ans;
int sel;
int n, k;

template <class T>
inline void read(T& val)
{
    char t;
    val = 0;
    for (t = getchar(); !isdigit(t); t = getchar())
        ;
    for (; isdigit(t); t = getchar())
        val = val * 10 + (t - '0');
}
void solve(const rang*, int);
/*-----operation for algorithm library-----*/
bool scmp(const rang* a, const rang* b)
{
    return a->l != b->l ? a->l < b->l : a->r > b->r;
}
bool lhcmp(const rang* a, const rang* b) //compare for min heap
{
    return a->r > b->r;
}
bool pscmp(const rang* a, const rang* b)
{
    return a->r > b->r;
}
const rang* op(const rang& a)
{
    return &a;
}

/*-----heap operation-----*/
inline void add(int p)
{
    if (heap[0]->r >= poi[p]->r)
        return;
    heap[k - 1] = poi[p];
    push_heap(heap, heap + k, lhcmp);
    pop_heap(heap, heap + k, lhcmp);
}

inline void solve(const rang* lv, int pos)
{
    num val = min(heap[0]->r, lv->r) - lv->l;
    if (val > ans)
    {
        ans = val;
        sel = pos;
    }
}
inline void spec_n()
{
    num l = 0, r = inf;
    for (rang* i = dat; i < dat + n; ++i)
    {
        l = max(l, i->l);
        r = min(r, i->r);
    }
    if (r - l > 0)
        printf("%d\n", r - l);
    else
        printf("%d\n", 0);
    for (int i = 1; i <= n; ++i)
        printf("%d ", i);
}
inline void spec_1()
{
    num ans = 0;
    int sel = 0;
    for (rang* i = dat; i < dat + n; ++i)
    {
        if (i->r - i->l > ans)
        {
            ans = i->r - i->l;
            sel = i->id;
        }
    }
    printf("%d\n%d", ans, sel);
}

int main()
{
    read(n);
    read(k);
    for (int i = 0; i < n; ++i)
    {
        read(dat[i].l);
        read(dat[i].r);
        dat[i].id = i + 1;
    }
    if (n == k)
    {
        spec_n();
        return 0;
    }
    if (k == 1)
    {
        spec_1();
        return 0;
    }

    transform(dat, dat + n, poi, op);
    sort(poi, poi + n, scmp);
    copy(poi, poi + k - 1, heap);
    make_heap(heap, heap + k - 1, lhcmp);

    for (int i = k - 1; i < n; ++i)
    {
        solve(poi[i], i);
        add(i);
    }

    printf("%d\n", ans);
    partial_sort_copy(poi, poi + sel, heap, heap + k - 1, pscmp);
    for (int i = 0; i < k - 1; ++i)
        printf("%d ", heap[i]->id);
    printf("%d", poi[sel]->id);
    return 0;
}