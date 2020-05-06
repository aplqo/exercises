#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 20;

unsigned int c[maxn + 10], p[maxn + 10], l[maxn + 10];

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    unsigned int n, m = stoi(argv[2]);
    in >> n;
    for (unsigned int i = 0; i < n; ++i)
        in >> c[i] >> p[i] >> l[i];
    in.close();
    for (unsigned int i = 0; i < n; ++i)
        --c[i];
    {
        const unsigned int e = *max_element(l, l + n) + 1;
        const unsigned int w = log10(m) + 2, wl = log10(e) + 3;
        for (unsigned int i = 0; i < e; ++i)
        {
            cout.width(wl);
            cout << i << ":";
            for (unsigned int j = 0; j < n; ++j)
            {
                cout.width(w);
                if (i <= l[j])
                {
                    cout << c[j];
                    c[j] = (c[j] + p[j]) % m;
                }
                else
                    cout << "X";
            }
            cout << endl;
        }
    }
    return 0;
}