#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 8000, maxm = 20000;

ifstream out;
struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxm * 2 + 10];
edge* head[maxn * 2 + 10];
unsigned int n, m;
bool conflict[maxn * 2 + 10], vis[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

inline void ReadData(const char* dat)
{
    ifstream in(dat);
    in >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        in >> a >> b;
        AddEdge(a, b);
        AddEdge(b, a);
    }
    in.close();
}
inline void TestEOF()
{
    if (out.eof())
    {
        cout << "Test failed! Output too short" << endl;
        exit(3);
    }
}
inline void TestNIE(const char* fans)
{
    ifstream ans(fans);
    if (isalpha(ans.peek()) || isalpha(out.peek()))
    {
        char ra = ans.peek(), ro = out.peek();
        TestEOF();
        if (ra == ro)
            exit(0);
        string os;
        out >> os;
        if (isalpha(ra))
        {
            cout << "Test failed! Expect NIE, read " << os << endl;
            exit(2);
        }
        else if (isalpha(ro))
        {
            cout << "Test failed! Expect number, read " << os << endl;
            exit(2);
        }
    }
    ans.close();
}

int main(int argc, char* argv[])
{
    out.open(argv[2]);
    ReadData(argv[1]);
    TestNIE(argv[3]);

    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int t, par;
        out >> t;
        TestEOF();
        par = (t + t % 2) / 2;
        if (conflict[t])
        {
            cout << "Test failed! People " << t << "is conflict with other." << endl;
            return 1;
        }
        if (vis[par])
        {
            cout << "Test failed! Party " << par << " has selected before." << endl;
            cout << "Fail people " << t << endl;
            return 1;
        }
        vis[par] = true;
        for (edge* i = head[t]; i; i = i->pre)
            conflict[i->to] = true;
    }
    out.close();
    return 0;
}