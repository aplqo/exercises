#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <cstdio>
using namespace std;
typedef int_fast32_t num;
constexpr int maxn = 1e5 + 5, maxl = 25;

int n;
num dat[maxn], table[maxn][maxl];
num lg[maxn];

template <class T>
void read(T& val)
{
    char t = 0;
    val = 0;
    while (!isdigit(t))
        t = getchar();
    while (isdigit(t))
    {
        val = val * 10 + t - '0';
        t = getchar();
    }
}
num getLog(int x)
{
    if (lg[x])
        return lg[x];
    return x > 1 ? lg[x] = getLog(x >> 1) + 1 : 0;
}
inline void init(num dat[maxn], int len)
{
    for (int i = 0; i < len; ++i)
        table[i][0] = dat[i];
    for (num j = 1; j <= getLog(n); ++j)
        for (int i = 0; i + (1 << j) <= n; ++i)
            table[i][j] = max(table[i][j - 1], table[i + (1 << j - 1)][j - 1]); // i + 1 <<j - 1 = (i + 1)<<(j -1 )
}
inline num query(int l, int r)
{
    num d = getLog(r - l + 1);
    return max(table[l][d], table[r - (1 << d) + 1][d]);
}

int main()
{
    int m;
    read(n);
    read(m);
    for_each(dat, dat + n, [](num(&t)) -> void { read(t); });
    init(dat, n);
    for (int i = 0; i < m; ++i)
    {
        int l, r;
        read(l);
        read(r);
        printf("%" PRIdFAST32 "\n", query(--l, --r));
    }
    return 0;
}