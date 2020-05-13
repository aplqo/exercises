#include <cmath>
#include <iostream>
#include <string>
using namespace std;
using num = unsigned int;
const num mod = 1e9 + 7;
const int maxn = 100;

num c[maxn + 10][maxn + 10], mv;

void init(const unsigned int m, const unsigned int n)
{
    for (unsigned int i = 0; i <= m; ++i)
        c[i][0] = 1;
    for (unsigned int j = 0; j <= n; ++j)
        c[0][j] = 1;
}
void fun(const unsigned int m, const unsigned int n)
{
    for (unsigned int i = 1; i <= m; ++i)
        for (unsigned int j = 1; j <= n; ++j)
        {
            c[i][j] = c[i - 1][j] + c[i][j - 1];
            mv = max(mv, c[i][j]);
        }
}

int main(int argc, char* argv[])
{
    unsigned int m = stoi(argv[1]), n = stoi(argv[2]);
    init(m, n);
    fun(m, n);
    const unsigned int w = log10(mv) + 1;
    for (unsigned int i = 0; i <= m; ++i)
    {
        for (unsigned int j = 0; j <= n; ++j)
        {
            cout.width(w);
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}