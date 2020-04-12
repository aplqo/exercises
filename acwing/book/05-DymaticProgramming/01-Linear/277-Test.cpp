#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const unsigned int maxn = 30;

enum ret : int
{
    Pass = 0,
    Short = 1,
    Angry = 2,
    Cookie = 3,
    Solution = 4,
    Zero = 5
};
struct child
{
    num_t g;
    unsigned int cnt, id;
} chl[maxn + 10];
unsigned int n, m;
unsigned int vout;

template <class T>
[[nodiscard]] T Read(ifstream& f)
{
    T ret;
    f >> ret;
    if (f.fail())
    {
        cout << "Fail: Output too short" << endl;
        exit(ret::Short);
    }
    return ret;
}
void ReadData(const char* fin)
{
    ifstream in(fin);
    in >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
    {
        in >> chl[i].g;
        chl[i].id = i;
    }
    in.close();
}
void ReadOutput(const char* fout, const unsigned int n)
{
    ifstream out(fout);
    vout = Read<unsigned int>(out);
    for (unsigned int i = 1; i <= n; ++i)
        chl[i].cnt = Read<unsigned int>(out);
    out.close();
}

void TestVal(const char* fans)
{
    ifstream ans(fans);
    unsigned int vans;
    ans >> vans;
    if (vout != vans)
    {
        cout << "Fail: Value mismatch. Read=" << vout << " Expect=" << vans << endl;
        exit(ret::Angry);
    }
    ans.close();
}
void TestSum(const unsigned int n, const unsigned int m)
{
    unsigned int s = 0;
    for (unsigned int i = 1; i <= n; ++i)
        s += chl[i].cnt;
    if (s != m)
    {
        cout << "Fail: Cookies sum mismatch. m=" << m << " read sum=" << s << endl;
        exit(ret::Cookie);
    }
}
void TestZero(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        if (!chl[i].cnt)
        {
            cout << "Fail: Child " << i << " get 0 cookies." << endl;
            exit(ret::Zero);
        }
}
void TestSolution(const unsigned int n)
{
    unsigned int v = 0;
    sort(chl + 1, chl + 1 + n, [](const child a, const child b) { return a.cnt > b.cnt; });
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int t = i;
        for (; chl[i].cnt == chl[t].cnt; ++i)
            v += (t - 1) * chl[i].g;
        --i;
    }
    if (v != vout)
    {
        cout << "Fail: Wrong solution. Read=" << v << " Expect=" << vout << endl;
        exit(ret::Solution);
    }
}

int main(int argc, char* argv[])
{
    ReadData(argv[1]);
    ReadOutput(argv[2], n);
    TestZero(n);
    TestVal(argv[3]);
    TestSum(n, m);
    TestSolution(n);
    return ret::Pass;
}
