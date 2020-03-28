#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const int maxn = 10000;

num_t f[maxn + 10] = { 1, 1 };

int main()
{
    unsigned int n;
    cin >> n;
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int j = 1; j <= i; ++j)
            f[i] += f[j - 1] * f[i - j];
    cout << f[n] << endl;
    return 0;
}