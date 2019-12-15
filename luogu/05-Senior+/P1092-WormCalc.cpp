#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
using namespace std;
const int maxn = 27;

bool nuvis[maxn];
int n, c[maxn];
struct digit
{
    inline void create(int val)
    {
        this->vis = true;
        this->nu = val;
        nuvis[val] = true;
    }
    inline void remove()
    {
        this->vis = false;
        nuvis[this->nu] = false;
    }
    inline void check(int* pos, int& su)
    {
        su = 0;
        fill(pos, pos + n, 0);
        for (int i = 0; i < rnum; ++i)
            if (rans[i]->vis && rvar[i]->vis)
            {
                ++su;
                int s = rans[i]->nu - rvar[i]->nu;
                if (s >= 0)
                    ++pos[s];
                if (s + n < n)
                    ++pos[s + n];
                if (s - 1 >= 0)
                    ++pos[s - 1];
                if (s - 1 + n < n)
                    pos[s - 1 + n]++;
            }
    }
    inline void conf(digit* var, digit* ans)
    {
        rvar[rnum] = var;
        rans[rnum] = ans;
        rnum++;
    }

    bool vis = false;
    digit *rans[maxn], *rvar[maxn];
    int nu, rnum = 0;
} ans[maxn];
digit* expr[maxn][3];

void dfs(digit* (*co)[3]);
template <int b, int dep, bool fl>
class trc
{
public:
    static inline void tr(digit* (*co)[3])
    {
        digit*(&col)[3] = *co;
        if (col[b]->vis)
        {
            trc<b, 2, false>::tr(co);
            return;
        }
        int pos[maxn], su;
        col[b]->check(pos, su);
        for (int i = 0; i < n; ++i)
            if ((!nuvis[i]) && pos[i] == su)
            {
                col[b]->create(i);
                trc<b + 1, dep + 1, true>::tr(co);
                col[b]->remove();
            }
    }
};
template <int b>
class trc<b, 1, true>
{
public:
    static inline void tr(digit* (*co)[3])
    {
        digit*(&col)[3] = *co;
        if (!col[2]->vis)
        {
            trc<b, 1, false>::tr(co);
            return;
        }
        if (col[b]->vis)
        {
            trc<b + 1, 2, false>::tr(co);
            return;
        }
        constexpr int ind = b == 1 ? 0 : 1;
        int s = col[2]->nu - c[co - expr] - col[ind]->nu;
        if (s >= 0 && (!nuvis[s]))
        {
            col[b]->create(s);
            trc<b, 2, false>::tr(co);
            col[b]->remove();
        }
        if (s + n < n && (!nuvis[s + n]))
        {
            col[b]->create(s + n);
            trc<b, 2, false>::tr(co);
            col[b]->remove();
        }
    }
};
template <int b, bool fl>
class trc<b, 2, fl>
{
public:
    static inline void tr(digit* (*co)[3])
    {
        digit*(&col)[3] = *co;
        int a = col[0]->nu + col[1]->nu + c[co - expr], m = a >= n ? (a -= n, 1) : 0;
        if ((col[2]->vis && col[2]->nu != a) || ((!col[2]->vis) && nuvis[a]))
            return;
        if (col[2]->vis)
        {
            c[co - expr + 1] = m;
            dfs(co + 1);
            return;
        }
        col[2]->create(a);
        c[co - expr + 1] = m;
        dfs(co + 1);
        col[2]->remove();
    }
};

void dfs(digit* (*co)[3])
{
    digit*(&col)[3] = *co;
    if (co == expr + n)
    {
        if (c[n])
            return;
        for (int i = 0; i < n; ++i)
            cout << ans[i].nu << " ";
        exit(0);
    }
    const bool v1 = col[0]->vis, v2 = col[1]->vis;
    if (v1 && v2)
        trc<0, 2, true>::tr(co);
    else if (!(v1 || v2))
        trc<0, 0, true>::tr(co);
    else if (v1)
        trc<1, 1, true>::tr(co);
    else
        trc<0, 1, true>::tr(co);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < 3; ++i)
    {
        string s;
        getline(cin, s);
        for (int j = n - 1, t = 0; j >= 0; --j, ++t)
            expr[t][i] = ans + s[j] - 'A';
    }
    for (digit*(*i)[3] = expr; i < expr + n; ++i)
    {
        digit*(&col)[3] = *i;
        col[0]->conf(col[1], col[2]);
        col[1]->conf(col[0], col[2]);
    }
    dfs(expr);
    return 0;
}