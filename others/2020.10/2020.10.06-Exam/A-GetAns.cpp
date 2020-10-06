#include <algorithm>
#include <fstream>
#include <numeric>
using namespace std;
const int maxn = 1e5;

struct Object
{
    unsigned int a, b;
} obj[maxn + 10];

bool check(const unsigned int seq[], const unsigned int len, unsigned long long v)
{
    for (const unsigned int* i = seq; i < seq + len; ++i)
    {
        if (v < obj[*i].a)
            return false;
        v = v - obj[*i].a + obj[*i].b;
    }
    return true;
}
bool enumerate(const unsigned int n, const unsigned int v)
{
    static unsigned int buf[maxn + 10];
    iota(buf, buf + n, 0);
    do
    {
        if (check(buf, n, v))
            return true;
    } while (next_permutation(buf, buf + n));
    return false;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int t;
    in >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned int n, v;
        in >> n >> v;
        for (Object* i = obj; i < obj + n; ++i)
            in >> i->a >> i->b;
        ans << (enumerate(n, v) ? "Yes" : "No") << "\n";
    }
    return 0;
}