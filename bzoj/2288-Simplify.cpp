#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    unsigned int n, m;
    int acc;
    bool typ;
    in >> n >> m;
    in >> acc;
    typ = acc > 0;
    for (unsigned int i = 1; i < n; ++i)
    {
        int t;
        in >> t;
        if (!t)
            continue;
        else if ((t > 0) ^ typ)
        {
            cout << acc << " ";
            acc = t;
            typ = t > 0;
        }
        else
            acc += t;
    }
    cout << acc << endl;
    return 0;
}