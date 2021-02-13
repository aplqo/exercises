#include "debug_tools/tester.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
const unsigned int maxn = 1e5;
using namespace apdebug::tester;

enum class ExitStatus
{
    Accept,
    ChangeTime,
    NotPermutation,
    Same,
    Output,
    Range
};
char dat[maxn + 10];

template <class... Args>
void fail(const ExitStatus val, Args&&... args)
{
    print(std::cout, args...);
    std::exit(static_cast<int>(val));
}
std::size_t readInput(const char* fin)
{
    std::ifstream in(fin);
    in >> (dat + 1);
    return std::strlen(dat + 1);
}
void checkAnswer(std::ifstream& out, const char* fans)
{
    std::ifstream ans(fans);
    unsigned int cur;
    ans >> cur;
    const unsigned int act = ReadOutput<unsigned int>(out);
    if (act != cur)
        fail(ExitStatus::ChangeTime, "Wrong answer. Expected=", cur, " Read=", act);
}
void checkSolution(std::ifstream& out, const std::size_t n)
{
    static unsigned int vis[maxn + 10];
    unsigned int lst = ReadOutput<unsigned int>(out);
    vis[lst] = true;
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int cur = ReadOutput<unsigned int>(out);
        if (cur > n)
            fail(ExitStatus::Range, "Wrong solution. ans[", i, "] = ", cur, " is greater than |S| = ", n);
        if (vis[cur])
            fail(ExitStatus::NotPermutation, "Wrong solution. ", cur, " visited twice. ans[", vis[cur], "] = ans[", i, "] = ", cur);
        if (dat[cur] == dat[lst])
            fail(ExitStatus::Same, "Wrong solution. Adjacent step is same. type[", lst, "] = type[", cur, "] = ", dat[cur]);
        lst = cur;
        vis[cur] = i;
    }
}

int main(int, char* argv[])
{
    std::ifstream out(argv[2]);
    try
    {
        checkAnswer(out, argv[3]);
        checkSolution(out, readInput(argv[1]));
    }
    catch (exceptions::ReadFail& s)
    {
        fail(ExitStatus::Output, "Invalid output");
    }
    return 0;
}