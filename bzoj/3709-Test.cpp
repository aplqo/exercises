#include "debug_tools/tester.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <utility>
using namespace std;
using namespace apdebug::tester;
using mon = pair<int, int>;
const int maxn = 100000;

enum class Return : int
{
    Accepted = 0,
    Vedirect = 1,
    Sequence = 2,
    Output = 3
};
ifstream out;
mon m[maxn + 10];
mon* sel[maxn + 10];
unsigned int n;
int z;

template <class... Args>
void fail(Return ret, Args... args)
{
    (cout << ... << args) << endl;
    exit(static_cast<int>(ret));
}
bool TestVedi(const char* fans)
{
    ifstream ans(fans);
    string av, ov;
    ans >> av;
    ans.close();
    ov = ReadOutput<string>(out);
    if (av != ov)
        fail(Return::Vedirect, "Wrong vedirect. Expected=", av, " Read=", ov, ".");
    return av == "NIE";
}
void TestSel()
{
    int c = z;
    for (unsigned int i = 0; i < n; ++i)
    {
        c -= sel[i]->first;
        if (c <= 0)
            fail(Return::Sequence, "Wrong order.z<0 on ", i, ".");
    }
}
void ReadDat(const char* c)
{
    ifstream in(c);
    in >> n >> z;
    for_each(m, m + n, [&in](mon& i) -> void { in >> i.first >> i.second; });
}
void ReadOut()
{
    for (unsigned int i = 0; i < n; ++i)
        sel[i] = m + ReadOutput<unsigned int>(out);
}

int main(int argc, char* argv[])
{
    out.open(argv[2]);
    try
    {
        if (TestVedi(argv[3]))
            return (int)Return::Accepted;
        ReadDat(argv[1]);
        ReadOut();
    }
    catch (exceptions::ReadFail)
    {
        fail(Return::Output, "Output too short.");
    }
    return (int)Return::Accepted;
}
