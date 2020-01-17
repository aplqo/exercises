#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100010, maxp = 1299715;

int primes[maxn], mdiv[maxp], cur = 0;

void GetPrimes()
{
    for (int i = 2; i < maxp; ++i)
    {
        if (!mdiv[i])
        {
            mdiv[i] = i;
            primes[cur++] = i;
        }
        for (int* j = primes; j < primes + cur && *j <= mdiv[i]; ++j)
            if (*j * i <= maxp)
                mdiv[(*j) * i] = *j;
            else
                break;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    GetPrimes();
    while (true)
    {
        int v;
        cin >> v;
        if (!v)
            break;
        int* p = lower_bound(primes, primes + cur, v);
        if (*p == v)
            cout << "0" << endl;
        else
            cout << (*p - *(p - 1)) << endl;
    }
    return 0;
}