#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string n;
    string::size_type k;
    cin >> n >> k;
    if (k >= n.size())
    {
        cout << "0" << endl;
        return 0;
    }
    while (k)
    {
        bool f = false;
        for (size_t i = 0; i < n.size() - 1 && k; ++i)
            if (n[i] > n[i + 1])
            {
                --k;
                f = true;
                n.erase(i, 1);
                break;
            }
        if (!f)
        {
            n.erase(n.end() - k, n.end());
            break;
        }
    }
    if (n[0] == '0')
    {
        size_t t = n.find_first_not_of('0');
        if (t > n.size())
            n = "0";
        else
            n.erase(n.begin(), n.begin() + t);
    }
    cout << n.c_str() << endl;
    return 0;
}