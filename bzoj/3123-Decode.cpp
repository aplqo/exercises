#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
using namespace std;

void copyWeight(ifstream& in, ostream& out, const unsigned int n)
{
    copy_n(istream_iterator<unsigned int>(in), n, ostream_iterator<unsigned int>(out, " "));
    out << endl;
}
void copyEdge(ifstream& in, ostream& out, const unsigned int m)
{
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        in >> x >> y;
        out << x << " " << y << endl;
    }
}
void copyOperate(ifstream& fin, ifstream& fans, ostream& out, const unsigned int t)
{
    unsigned int lst = 0;
    for (unsigned int i = 0; i < t; ++i)
    {
        char op;
        unsigned int x, y;
        fin >> op >> x >> y;
        x ^= lst;
        y ^= lst;
        out << op << " " << x << " " << y;
        if (op == 'Q')
        {
            unsigned int k;
            fin >> k;
            k ^= lst;
            out << " " << k;
            fans >> lst;
        }
        out << endl;
    }
}

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]), fans(argv[2]);
    ofstream fout;
    if (argc > 3)
        fout.open(argv[3]);
    ostream& out = argc > 3 ? fout : cout;
    {
        unsigned int ci;
        fin >> ci;
        out << ci << endl;
    }
    unsigned int n, m, t;
    fin >> n >> m >> t;
    out << n << " " << m << " " << t << endl;
    copyWeight(fin, out, n);
    copyEdge(fin, out, m);
    copyOperate(fin, fans, out, t);
    return 0;
}