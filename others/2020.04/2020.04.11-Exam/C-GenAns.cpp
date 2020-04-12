#include <fstream>
using namespace std;
using num = unsigned long long;
const int maxn = 1e5;

num a[maxn + 10];

num query(const unsigned int l, const unsigned int r, num x)
{
    for (unsigned int i = l; i <= r; ++i)
        x %= a[i];
    return x;
}
int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    unsigned int n, m;
    in >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
        in >> a[i];
    for (unsigned int j = 0; j < m; ++j)
    {
        num x;
        unsigned int l, r;
        in >> x >> l >> r;
        out << query(l, r, x) << endl;
    }
    in.close();
    out.close();
    return 0;
}