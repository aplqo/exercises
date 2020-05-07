#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    const unsigned int maxf = argv ? stoi(argv[2]) : 100;
    unsigned int fail = 0;
    while (in)
    {
        int c, x, a, m;
        in >> c >> x >> a >> m;
        if (in.eof())
            break;
        if ((c - x * a) % m)
        {
            // c=ax+my
            if (fail < maxf)
                cout << "Fail: c=" << c << " x=" << x << " p=" << a << " m=" << m << endl;
            ++fail;
        }
    }
    if (maxf)
        cout << "GCD test failed. Found " << fail << " errors." << endl;
    return fail ? 1 : 0;
}