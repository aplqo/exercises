#include "debug_tools/tester.h"
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <tuple>
using namespace std;
using namespace apdebug::tester;
const unsigned int maxd = 1000, maxn = 100;

enum class State
{
    Accepted = 0,
    Output = 1, // output too short
    Value = 2
};

unsigned long long c[maxd + 10][maxn + 10], w[maxn + 10][maxn + 10];
unsigned long long tmp[maxd + 10][maxd + 10], ans[maxd + 10][maxn + 10];

template <class... Args>
[[noreturn]] void quitf(State ret, Args... args)
{
    print(std::cout, args...);
    exit(static_cast<int>(ret));
}
template <class T, unsigned int my>
void readMatrix(T read, const unsigned int x, const unsigned int y, unsigned long long m[][my])
{
    for (unsigned int i = 0; i < x; ++i)
        for (unsigned int j = 0; j < y; ++j)
            m[i][j] = read();
}
tuple<unsigned int, unsigned int, unsigned long long> readInput(const char* f)
{
    ifstream in(f);
    unsigned int n, d;
    unsigned long long p;
    in >> n >> d >> p;
    const auto read = [&in]() { unsigned long long t;in>>t;return t; };
    readMatrix(read, n, n, w);
    readMatrix(read, d, n, c);
    return make_tuple(n, d, p);
}
void readOut(const char* f, const unsigned int n, const unsigned int d)
{
    readMatrix(bind(ReadOutput<unsigned long long>, ifstream(f)), d, n, ans);
}
void calcC(const unsigned int n, const unsigned int d, const unsigned int p)
{
    for (unsigned int i = 0; i < d; ++i)
        for (unsigned int j = 0; j < n; ++j)
            for (unsigned int k = 0; k < n; ++k)
                tmp[i][j] = (tmp[i][j] + (w[j][k] * ans[i][k]) % p) % p;
}
void test(const unsigned int n, const unsigned int d)
{
    for (unsigned int i = 0; i < d; ++i)
        for (unsigned int j = 0; j < n; ++j)
            if (c[i][j] != tmp[i][j])
                quitf(State::Value, "Wrong answer on day ", i, " plan ", j, ". Expected=", c[i][j], " Read=", tmp[i][j]);
}

int main(int argc, char* argv[])
{
    auto [n, d, p] = readInput(argv[1]);
    try
    {
        readOut(argv[2], n, d);
    }
    catch (exceptions::ReadFail)
    {
        quitf(State::Output, "Output too short");
    }
    calcC(n, d, p);
    test(n, d);
    return static_cast<int>(State::Accepted);
}