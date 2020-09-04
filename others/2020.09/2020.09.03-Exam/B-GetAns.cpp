#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 300000, maxm = 300000;

struct Edge
{
    unsigned int from, to;
} tre[maxn + 1], non[maxm + 1];
unsigned int n, m;

inline istream& operator>>(istream& is, Edge& e)
{
    return is >> e.from >> e.to;
}
namespace dsu
{
    unsigned int fa[maxn + 1], siz[maxn + 1];
    static void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
}
bool connect(const Edge* banTree, const Edge* banNon)
{
    dsu::init(n);
    for (const Edge* i = tre; i < tre + n - 1; ++i)
        if (i != banTree)
            dsu::merge(i->from, i->to);
    for (const Edge* i = non; i < non + m; ++i)
        if (i != banNon)
            dsu::merge(i->from, i->to);
    return dsu::siz[dsu::find(1)] == n;
}
unsigned long long solve(const unsigned int n, const unsigned int m)
{
    unsigned long long ret = 0;
    for (const Edge* i = tre; i < tre + n - 1; ++i)
        for (const Edge* j = non; j < non + m; ++j)
            ret += !connect(i, j);
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    in >> n >> m;
    copy_n(istream_iterator<Edge>(in), n - 1, tre);
    copy_n(istream_iterator<Edge>(in), m, non);
    ans << solve(n, m) << "\n";
    return 0;
}