#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]), out(argv[2]);
    unsigned long long n, ret;
    in >> n;
    out >> ret;
    in.close();
    out.close();
    if (ret % n)
    {
        cout << "Test failed remainer: " << ret % n << endl;
        return 1;
    }
    else
    {
        cout << "Test passed" << endl;
        return 0;
    }
}
