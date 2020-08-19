#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5;
constexpr long long inf = LLONG_MAX / 3;
using pair_t = pair<long long, unsigned int>;

char outbuf[maxq * 15 + 1000], *outpos = outbuf, inbuf[maxn * 10 + maxq * 20 + 1000], *inpos = inbuf;
unsigned int readUInt()
{
    unsigned int ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = *(inpos++);
    }
    return ret;
}
void write(long long v)
{
    outpos += sprintf(outpos, "%lld\n", v);
}

enum
{
    Add = 0,
    Rem = 1
};
multiset<pair_t> st[2];
long long len[maxn + maxq + 1];
int cut[maxn + maxq + 1], k;

inline long long cost(const long long v, const int k)
{
    const auto sq = [](long long x) { return x * x; };
    const long long t = v % (k + 1), av = v / (k + 1);
    return sq(av + 1) * t + sq(av) * (k + 1 - t);
}
inline pair_t nxt(unsigned int pos, const int delt)
{
    const int c = cut[pos] + delt;
    if (c >= 0 && c < len[pos])
        return pair_t(cost(len[pos], c) - cost(len[pos], cut[pos]), pos);
    else
        return pair_t(inf, pos);
}
template <bool typ, int delt>
long long changeK()
{
    const auto [val, pos] = *st[typ].begin();
    st[typ].erase(st[typ].begin());
    st[!typ].erase(nxt(pos, -delt));
    cut[pos] += delt;
    st[Add].insert(nxt(pos, 1));
    st[Rem].insert(nxt(pos, -1));
    k += delt;
    return val;
}
inline long long addObject(const unsigned int p, const long long v)
{
    len[p] = v;
    st[Add].insert(nxt(p, 1));
    st[Rem].insert(nxt(p, -1));
    return cost(len[p], 0);
}
inline long long updateAns()
{
    long long ret = 0;
    while (st[Add].begin()->first + st[Rem].begin()->first < 0)
        ret += changeK<Add, 1>() + changeK<Rem, -1>();
    return ret;
}

// test code
void printMS(const multiset<pair_t>& mp)
{
    for (pair_t i : mp)
        clog << i.second << " cut=" << cut[i.second] << " " << i.first << '\n';
    clog << endl;
}

static long long init(const unsigned int n)
{
    long long ret = 0;
    for (unsigned int i = 0; i < n; ++i)
        ret += addObject(i, len[i]);
    const int exk = k;
    k = 0;
    while (k < exk)
        ret += changeK<Add, 1>();
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    unsigned int n = readUInt(), q = readUInt();
    k = readUInt();
    generate_n(len, n, readUInt);
    long long ans = init(n);
    write(ans);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int op = readUInt();
        switch (op)
        {
        case 1:
            write(ans += changeK<Add, 1>());
            break;
        case 2:
            write(ans += changeK<Rem, -1>());
            break;
        case 3:
            long long l = readUInt();
            ans += addObject(n++, l);
            ans += updateAns();
            write(ans);
            break;
        }
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}