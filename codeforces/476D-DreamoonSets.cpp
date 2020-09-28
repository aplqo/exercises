#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <sstream>
using namespace std;

unsigned long long generate(ostream& os, const unsigned int n, const unsigned int k)
{
    constexpr unsigned int off[] = { 1, 2, 3, 5 };
    unsigned long long ret = 0, acc = 0;
    for (unsigned int i = 0; i < n; ++i, acc += 6)
    {
        for (unsigned int j : off)
            os << (acc + j) * k << " ";
        os << "\n";
    }
    return (acc - 6 + off[3]) * k;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    ostringstream os;
    cout << generate(os, n, k) << "\n";
    cout << os.str();
    return 0;
}