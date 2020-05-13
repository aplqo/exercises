#include <fstream>
using namespace std;
using num = unsigned long long;

num fun(const unsigned int x, const unsigned int y)
{
    num ret = 0;
    const unsigned int e = ((y - 1) >> 1) * x;
    for (num i = x; i <= e; i += x)
        ret += i / y;
    return ret;
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int p, q;
    in >> p >> q;
    ans << fun(p, q) + fun(q, p) << endl;
    return 0;
}