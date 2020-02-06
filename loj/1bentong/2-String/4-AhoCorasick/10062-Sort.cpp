#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 3e4;

unsigned int n;
string* strs;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    in >> n;
    strs = new string[n];
    for (string* i = strs; i < strs + n; ++i)
        in >> *i;
    in.close();
    sort(strs, strs + n);
    for (string* i = strs; i < strs + n; ++i)
        cout << *i << endl;
    delete[] strs;
    in.close();
    return 0;
}