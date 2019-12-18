#include <fstream>
#include <algorithm>
#include <iostream>
#include <numeric>
const int maxn = 70;
using namespace std;

ifstream dat;
int a[maxn];
int main(int argc, char *argv[])
{
    dat.open(argv[1]);
    int n;
    dat >> n;
    for_each(a, a + n, [](int &i) -> void { dat >> i; });
    sort(a, a + n, [](const int a, const int b) -> bool { return a > b; });
    cout << "Sum: " << accumulate(a, a + n, 0) << endl;
    cout << "Sorted data: ";
    for_each(a, a + n, [](int &i) -> void { cout << i << " "; });
    return 0;
}