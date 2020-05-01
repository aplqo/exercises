/*Luogu team T132119: Prime squence*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
using namespace std;
using num_t = int;
const int maxn = 2000, maxq = 6000;

struct edge
{
    edge *to[maxn + 10], *from;
    unsigned int cnt = 0;
    num_t val, sum = 0;
    unsigned int len = 0;

    void add(edge* const v)
    {
        to[cnt++] = v;
    }
} ed[maxn + 10];
bool mat[maxn + 10][maxn + 10];
unsigned int tmp[maxn + 10];
unsigned int mindiv[maxq + 10], primes[maxq + 10], *pcur = primes;

void eular(const unsigned int ma)
{
    for (unsigned int i = 2; i <= ma; ++i)
    {
        if (!mindiv[i])
        {
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (unsigned int* j = primes; j < pcur && *j <= mindiv[i] && *j * i <= ma; ++j)
            mindiv[*j * i] = *j;
    }
}
bool isPrime(const unsigned int val)
{
    for (unsigned int* i = primes; i < pcur; ++i)
        if (val % *i)
            return false;
    return true;
}
void fun(const unsigned int n)
{
    for (edge* i = ed; i < ed + n; ++i)
        for (edge* j = i + 1; j < ed + n; ++j)
            if (isPrime(i->val + j->val))
                j->add(i);
    for (edge* i = ed; i < ed + n; ++i)
    {
        for (unsigned int j = 0; j < i->cnt; ++j)
        {
            const unsigned int cl = i->to[j]->len + 1;
            const num_t cs = i->to[j]->sum + i->val;
            if (i->len < cl || (i->len == cl && i->sum < cs))
            {
                i->len = cl;
                i->val = cs;
                i->from = i->to[j];
            }
        }
    }
}
unsigned int getAns(const unsigned int n)
{
    unsigned int ret = 0;
    edge* sel = ed;
    for (edge* i = ed + 1; i < ed + n; ++i)
        if (i->len > sel->len || (i->len == sel->len && i->sum > sel->sum))
            sel = i;
    for (edge* i = sel; i; i = i->from)
        tmp[ret++] = i->val;
    return sel->val;
}
int main()
{
    unsigned int n;
    cin >> n;
    for (unsigned int* i = tmp; i < tmp + n; ++i)
        cin >> *i;
    sort(tmp, tmp + n, greater<unsigned int>());
    for (unsigned int i = 0; i < n; ++i)
        ed[i].val = tmp[i];
    eular(tmp[0] + tmp[1]);
    fun(n);
    cout << getAns(n) << endl;
    for (unsigned int* i = tmp; i < tmp + n; ++i)
        cout << *i << " ";
    return 0;
}