#include <iostream>
#include <numeric>
using namespace std;

void fun(uint16_t x)
{
    uint16_t t = 0x8000;
    bool f = false;
    for (uint16_t i = 15; i < 16; i--, t >>= 1)
    {
        if (!(x & t)) continue;
        if (f) cout << '+';
        f = true;
        switch (i)
        {
        case 0: cout << "2(0)"; break;
        case 1: cout << "2"; break;
        case 2: cout << "2(2)"; break;
        default:
            cout << "2(";
			fun(i);
            cout << ")";
        }
    }
}
int main()
{
    uint16_t x;
    cin >> x;
    fun(x);
    return 0;
}