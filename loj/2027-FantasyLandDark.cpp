#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
const unsigned int maxn = 17;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                               \
    inline friend Number operator x(const Number l, const Number r) { return l.val x r.val; } \
    inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }
    inline bool isZero() const { return !val; }
    inline Number operator-() const
    {
        Number ret;
        ret.val = val ? mod - val : 0;
        return ret;
    }
    inline void operator++() { val = val == mod - 1 ? 0 : val + 1; }
    inline void operator--() { val = val ? val - 1 : mod - 1; }

private:
    unsigned long long val = 0;
};
struct Edge
{
    unsigned int u, v;
};
std::vector<Edge> ed[maxn + 10];
Number mat[maxn + 10][maxn + 10];
unsigned int popcnt[1u << maxn], lg[1u << maxn];

Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
inline Number inverse(const Number a)
{
    return quickPow(a, mod - 2);
}
void addCompany(const std::vector<Edge>& v)
{
    for (const auto& i : v)
    {
        ++mat[i.u][i.v];
        ++mat[i.v][i.u];
        --mat[i.u][i.u];
        --mat[i.v][i.v];
    }
}

void clearMatrix(const unsigned int n)
{
    static constexpr Number zero;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            mat[i][j] = zero;
}
Number gaussian(const unsigned int n)
{
    Number rat = 1;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (mat[i][i].isZero())
        {
            for (unsigned int j = i + 1; j < n; ++j)
                if (!mat[j][i].isZero())
                {
                    std::swap_ranges(mat[i] + i, mat[i] + n, mat[j] + i);
                    rat = -rat;
                    break;
                }
        }
        if (mat[i][i].isZero())
            return 0;
        const Number inv = -inverse(mat[i][i]);
        for (unsigned int j = i + 1; j < n; ++j)
            if (!mat[j][i].isZero())
            {
                const Number r = inv * mat[j][i];
                for (unsigned int k = i; k < n; ++k)
                    mat[j][k] += mat[i][k] * r;
            }
    }
    for (unsigned int i = 0; i < n; ++i)
        rat *= mat[i][i];
    return rat;
}
#define lowbit(x) ((x) & -(x))
static void init(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        lg[1u << i] = i;
    const unsigned int msk = 1u << n;
    for (unsigned int i = 1; i < msk; ++i)
        popcnt[i] = popcnt[i ^ lowbit(i)] + 1;
}
Number solve(const unsigned int n)
{
    static constexpr unsigned long long pmsk = UINT_MAX - 1;
    Number ret = 0;
    const unsigned int msk = 1u << (n - 1), p = n & pmsk;
    for (unsigned int i = 1; i < msk; ++i)
    {
        clearMatrix(n - 1);
        for (unsigned int j = i; j;)
        {
            const unsigned int lb = lowbit(j);
            addCompany(ed[lg[lb]]);
            j ^= lb;
        }
        if ((p - popcnt[i]) & 0x01)
            ret += -gaussian(n - 1);
        else
            ret += gaussian(n - 1);
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n;
    std::cin >> n;
    for (unsigned int i = 0; i < n - 1; ++i)
    {
        unsigned int m;
        std::cin >> m;
        ed[i].reserve(m);
        for (unsigned int j = 0; j < m; ++j)
        {
            unsigned int x, y;
            std::cin >> x >> y;
            ed[i].push_back(Edge { x - 1, y - 1 });
        }
    }
    init(n);
    std::cout << solve(n) << "\n";
    return 0;
}