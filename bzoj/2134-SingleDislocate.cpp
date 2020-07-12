#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;
const unsigned int maxn = 10000000;
using real = double;

unsigned int a[maxn + 1];

int main()
{
    unsigned int n;
    {
        int A, B, C;
        scanf("%u%d%d%d%u", &n, &A, &B, &C, a);
        for (unsigned int i = 1; i < n; i++)
            a[i] = ((long long)a[i - 1] * A + B) % 100000001;
        for (unsigned int i = 0; i < n; i++)
            a[i] = a[i] % C + 1;
    }
    real ans = 0;
    for (unsigned int i = 0; i < n; ++i)
        ans += real(1) / max(a[i], a[(i + 1) % n]);
    printf("%.3lf", ans);
    return 0;
}