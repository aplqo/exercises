#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
constexpr int maxn = 1000, maxm = maxn * maxn - 1, maxl = 1000;

struct edge
{
    edge() = default;
    edge(unsigned int f, unsigned int t, unsigned int v)
        : from(f)
        , to(t)
        , val(v)
    {
    }
    unsigned int from, to, val;
} ed[maxm + 10], *cur = ed;
unsigned int f[maxn + 10];
unsigned int mat[maxn + 10][maxn + 10];

mt19937_64 rnd;

unsigned int GetRandom(const unsigned int a, const unsigned int b)
{
    uniform_int_distribution<unsigned int> dis(a, b);
    return dis(rnd);
}
void CreateGraph(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int to = GetRandom(1, i - 1);
        mat[to][i] = GetRandom(1, maxl);
        mat[i][to] = mat[to][i];
    }
    for (unsigned int i = n; i <= m; ++i)
    {
        unsigned int a, b;
        do
        {
            a = GetRandom(1, n);
            b = GetRandom(1, n);
        } while (a == b || mat[a][b]);
        mat[a][b] = GetRandom(1, maxl);
        mat[b][a] = mat[a][b];
    }
}
void GetEdge(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = i + 1; j <= n; ++j)
            if (mat[i][j])
            {
                *cur = edge(f[i], f[j], mat[i][j]);
                ++cur;
            }
    shuffle(ed, cur, rnd);
}
int main(int argc, char* argv[])
{
    rnd.seed(random_device()());

    unsigned int n = atoi(argv[1]), m = GetRandom(n - 1, n * (n - 1) / 2);
    CreateGraph(n, m);

    iota(f, f + n + 1, 0);
    shuffle(f + 1, f + n + 1, rnd);

    GetEdge(n);
    ofstream out(argv[2]);
    out << n << " " << m << endl;
    for (edge* i = ed; i < cur; ++i)
        out << i->from << " " << i->to << " " << i->val << endl;
    out.close();
    return 0;
}
