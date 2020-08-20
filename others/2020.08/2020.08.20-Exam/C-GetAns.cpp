#include <algorithm>
#include <fstream>
using namespace std;

unsigned long long dfs(const unsigned long long x, const unsigned long long base)
{
    if (!x)
        return 1;
    if (x < base)
        return 0;
    unsigned long long ret = 0;
    for (unsigned int i = 0; x >= base * i; ++i)
        ret += dfs(x - i * base, base << 1);
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int t;
    in >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned long long v;
        in >> v;
        ans << dfs(v, 1) << endl;
    }
    return 0;
}