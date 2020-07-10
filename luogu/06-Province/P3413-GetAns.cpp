#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxl = 18;

inline unsigned int toDigit(unsigned long long x, unsigned int d[])
{
    unsigned int ret = 0;
    do
    {
        d[++ret] = x % 10;
        x /= 10;
    } while (x);
    return ret;
}
bool test(unsigned long long x)
{
    unsigned int d[maxl + 2];
    const unsigned int l = toDigit(x, d);
    d[l + 1] = d[l + 2] = 10;
    for (unsigned int i = 1; i <= l; ++i)
        if (d[i] == d[i + 1] || d[i] == d[i + 2])
            return true;
    return false;
}
unsigned long long count(const unsigned long long l, const unsigned long long r)
{
    unsigned long long ret = 0;
    for (unsigned long long i = l; i <= r; ++i)
        ret += test(i);
    return ret;
}
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        ifstream in(argv[1]);
        unsigned long long l, r;
        in >> l >> r;
        ofstream(argv[2]) << count(l, r) << endl;
    }
    {
        while (true)
        {
            unsigned long long l, r;
            cout << "Cute Number>";
            cout.flush();
            cin >> l >> r;
            if (cin.fail())
                break;
            cout << count(l, r) << endl;
        }
    }
    return 0;
}