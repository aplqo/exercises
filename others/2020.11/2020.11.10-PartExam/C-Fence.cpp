#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
using namespace std;
const unsigned int maxn = 2000;

struct Square
{
    unsigned int x1, y1, x2, y2;

    inline bool operator<(const Square& r) const
    {
        if (x1 != r.x1)
            return x1 < r.x1;
        else if (x2 != r.x2)
            return x2 < r.x2;
        else if (y1 != r.y1)
            return y1 < r.y1;
        else
            return y2 < r.y2;
    }
};
map<Square, unsigned long long> sq;
unsigned int n, m;

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    unsigned long long val[maxn + 10][maxn + 10];
    void prefixXor(const unsigned int x, const unsigned int y, const unsigned int v)
    {
        for (unsigned int i = x; i <= n; i += lowbit(i))
            for (unsigned int j = y; j <= m; j += lowbit(j))
                val[i][j] ^= v;
    }
    unsigned long long prefixSum(const unsigned int x, const unsigned int y)
    {
        unsigned long long ret = 0;
        for (unsigned int i = x; i; i -= lowbit(i))
            for (unsigned int j = y; j; j -= lowbit(j))
                ret ^= val[i][j];
        return ret;
    }
    void xorAdd(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned long long v)
    {
        prefixXor(x1, y1, v);
        prefixXor(x1, y2 + 1, v);
        prefixXor(x2 + 1, y1, v);
        prefixXor(x2 + 1, y2 + 1, v);
    }
#undef lowbit
}
namespace Rand
{
    ranlux24 rnd((random_device())());
    uniform_int_distribution<unsigned long long> dis(0);

    inline unsigned long long getRandom()
    {
        return dis(rnd);
    }
}

inline void addSquare(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
    const unsigned long long w = Rand::getRandom();
    sq[Square { x1, y1, x2, y2 }] = w;
    FTree::xorAdd(x1, y1, x2, y2, w);
}
inline void removeSquare(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
    const auto it = sq.find(Square { x1, y1, x2, y2 });
    FTree::xorAdd(x1, y1, x2, y2, it->second);
    sq.erase(it);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int q;
    cin >> n >> m >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int p, x1, y1, x2, y2;
        cin >> p >> x1 >> y1 >> x2 >> y2;
        switch (p)
        {
        case 1:
            addSquare(x1, y1, x2, y2);
            break;
        case 2:
            removeSquare(x1, y1, x2, y2);
            break;
        case 3:
            cout << (FTree::prefixSum(x1, y1) == FTree::prefixSum(x2, y2) ? "Yes" : "No") << "\n";
            break;
        }
    }
    return 0;
}