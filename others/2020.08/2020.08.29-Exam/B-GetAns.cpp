#include <algorithm>
#include <climits>
#include <fstream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned int inf = UINT_MAX;

struct Pill
{
    long long a, b;
} p[maxn + 1];
long long c[maxn + 1];

unsigned int test(const unsigned int n, const long long l, const unsigned int seq[])
{
    long long water = 0, pos = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        pos += p[seq[i]].a;
        if (pos >= l)
            return i + 1;
        pos -= p[seq[i]].b;
        water += c[i];
        if (water >= pos)
            return inf;
    }
    return inf;
}
unsigned int isSucc(const unsigned int n, const unsigned int l)
{
    static unsigned int seq[maxn + 1];
    unsigned int ret = inf;
    iota(seq, seq + n, 0);
    do
    {
        ret = min(ret, test(n, l, seq));
    } while (next_permutation(seq, seq + n));
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n;
    long long l;
    in >> n >> l;
    for (unsigned int i = 0; i < n; ++i)
        in >> p[i].a >> p[i].b;
    for (long long* i = c; i < c + n; ++i)
        in >> *i;
    const unsigned int v = isSucc(n, l);
    if (v == inf)
        ans << "-1\n";
    else
        ans << v << "\n";
    return 0;
}