#include "debug_tools/tester.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
using namespace apdebug::tester;

enum
{
    Accept = 0,
    Sum = 1,
    Solution = 2,
    Output = 3,
    Increase = 4
};
template <class... Args>
void fail(const int rv, Args... args)
{
    print(cout, args...);
    exit(rv);
}

unsigned long long testSum(ifstream& out, const char* ans)
{
    ifstream fans(ans);
    unsigned long long ansv, res;
    fans >> ansv;
    res = ReadOutput<unsigned long long>(out);
    if (ansv != res)
        fail(Sum, "Sum not match! Read=", res, " Expected=", ansv);
    return ansv;
}
void testSeq(ifstream& out, const char* in, const long long ansv)
{
    ifstream fin(in);
    unsigned int n;
    fin >> n;
    int lst = ReadOutput<int>(out);
    unsigned long long acc = 0;
    {
        int t;
        fin >> t;
        acc += abs(t - lst);
    }
    for (unsigned int i = 2; i <= n; ++i)
    {
        int nv = ReadOutput<int>(out), av;
        fin >> av;
        if (!(nv > lst))
            fail(Increase, "Output sequence not inceease. b[", i - 1, "]=", lst, " b[", i, "]=", nv);
        acc += abs(av - nv);
        lst = nv;
    }
    if (acc != ansv)
        fail(Solution, "Output sequence sum not correct. Expect=", ansv, " Actually=", acc);
}
int main(int, char* argv[])
{
    ifstream out(argv[2]);
    try
    {
        testSeq(out, argv[1], testSum(out, argv[3]));
    }
    catch (exceptions::ReadFail& e)
    {
        fail(Output, "Output too short!");
    }
    return Accept;
}