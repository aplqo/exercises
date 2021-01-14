#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

int main()
{
    unsigned int n;
    cin >> n;
    switch (n % 3)
    {
    case 0:
        cout << (n / 3) * 7 << "\n";
        break;
    case 1:
        cout << (n / 3 - 1) * 7 + 4 << "\n";
        break;
    case 2:
        cout << (n / 3) * 7 + 1 << "\n";
        break;
    }
    return 0;
}