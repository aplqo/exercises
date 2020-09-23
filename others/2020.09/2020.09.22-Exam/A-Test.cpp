#include "debug_tools/tester.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
using namespace apdebug::tester;
const unsigned int maxn = 1e5;

enum Ret
{
    Accept = 0,
    Count = 1,
    Output = 2,
    Range = 3,
    Sum = 4
};
int a[maxn + 10];

unsigned int readInput(const char* fil)
{
    ifstream in(fil);
    unsigned int n;
    in >> n;
    copy_n(istream_iterator<unsigned int>(in), n, a + 1);
    return n;
}
unsigned int testCount(ifstream& out, const char* ans)
{
    ifstream fans(ans);
    unsigned int c1, c2;
    fans >> c1;
    c2 = ReadOutput<unsigned int>(out);
    if (c1 != c2)
    {
        cout << "Wrong answer. Read=" << c2 << " Expect=" << c1 << "\n";
        exit(Count);
    }
    return c1;
}
void testSum(ifstream& out, const unsigned int n, const unsigned int cn)
{
    static int seq[maxn + 10];
    for (unsigned int i = 0; i < cn; ++i)
    {
        const unsigned int a = ReadOutput<unsigned int>(out), b = ReadOutput<unsigned int>(out);
        if (a > b)
        {
            cout << "Range invalid on operate " << i << ". l=" << a << " r=" << b << "\n";
            exit(Range);
        }
        ++seq[a];
        --seq[b + 1];
    }
    partial_sum(seq + 1, seq + n + 1, seq + 1);
    for (unsigned int i = 1; i <= n; ++i)
        if (seq[i] != a[i])
        {
            cout << "Sum not equal. a[" << i << "]=" << a[i] << " out[" << i << "]=" << seq[i] << "\n";
            exit(Sum);
        }
}

int main(int, char* argv[])
{
    const unsigned int n = readInput(argv[1]);
    ifstream out(argv[2]);
    try
    {
        const unsigned int cn = testCount(out, argv[3]);
        testSum(out, n, cn);
    }
    catch (exceptions::ReadFail& e)
    {
        cout << "Output invalid!\n";
        exit(Output);
    }
    return Accept;
}