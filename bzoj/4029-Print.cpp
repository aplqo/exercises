#include <climits>
#include <iostream>
using namespace std;

unsigned int toDigits(unsigned int x, unsigned int res[])
{
    unsigned int pos = 0;
    do
    {
        res[pos++] = x % 10;
        x /= 10;
    } while (x);
    return pos;
}
unsigned int cost(const unsigned int x)
{
    static unsigned int buf[10];
    const unsigned int len = toDigits(x, buf);
    for (unsigned int i = 0; i < len; ++i)
        if (buf[i])
            return 2 * (len - i) - (buf[i] == 5);
    return 0;
}
unsigned int find(const unsigned int l, const unsigned int r)
{
    unsigned int ret = 0, cst = UINT_MAX;
    for (unsigned int i = l; i <= r; ++i)
    {
        const unsigned int nv = cost(i);
        if (nv < cst)
        {
            ret = i;
            cst = nv;
        }
    }
    return ret;
}
int main()
{
    while (true)
    {
        cout << "Pricing>" << flush;
        unsigned int l, r;
        cin >> l >> r;
        if (cin.fail())
            break;
        cout << find(l, r) << endl;
    }
    return 0;
}