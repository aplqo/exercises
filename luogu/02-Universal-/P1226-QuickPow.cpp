#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <iostream>
using namespace std;
typedef unsigned long long numType;

numType mo;

numType QuickPow(numType a, numType p)
{
    if (p == 1)
        return a % mo;
    if (p == 0)
        return 1;
    numType m = QuickPow(a, p >> 1);
    if (p % 2)
        return a * m * m % mo;
    else
        return m * m % mo;
}
int main()
{
    ios_base::sync_with_stdio(false);
    numType b, p;
    cin >> b >> p >> mo;
    cout << b << "^" << p << " mod " << mo << "=" << QuickPow(b, p) % mo << endl;
    return 0;
}