#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100, maxk = 100;

long long mat[maxn * maxk + 1][maxn * maxk + 1];

void buildMatrix(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 0; i < n * k; ++i)
        for (unsigned int j = i + 1; j < n * k; ++j)
            if (i % n != j % n)
            {
                --mat[i][j];
                --mat[j][i];
                ++mat[i][i];
                ++mat[j][j];
            }
}
int main()
{
    unsigned int n, k;
    cin >> n >> k;
    buildMatrix(n, k);
    for (unsigned int i = 0; i < n * k - 1; ++i)
    {
        cout << "{" << mat[i][0];
        for (unsigned int j = 1; j < n * k - 1; ++j)
            cout << "," << mat[i][j];
        cout << "}," << endl;
    }
    return 0;
}