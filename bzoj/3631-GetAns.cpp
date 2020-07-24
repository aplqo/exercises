#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 300000;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
unsigned long long val[maxn + 1];
unsigned int a[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
bool dfs(const unsigned int x, const unsigned int f, const unsigned int y)
{
    if (x == y)
        return true;
    for (const edge* i = head[x]; i; i = i->pre)
        if (i->to != f && dfs(i->to, x, y))
        {
            ++val[x];
            return true;
        }
    return false;
}

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n;
    in >> n;
    copy_n(istream_iterator<unsigned int>(in), n, a);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y;
        in >> x >> y;
        addEdge(x, y);
        addEdge(y, x);
    }
    for (unsigned int i = 0; i < n - 1; ++i)
        dfs(a[i], 0, a[i + 1]);
    copy_n(val + 1, n, ostream_iterator<unsigned int>(ans, "\n"));
    return 0;
}