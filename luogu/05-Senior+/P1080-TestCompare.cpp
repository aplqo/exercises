#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
const int maxn = 1005;

string num[maxn], oans;
bool cmp(const string& a, const string& b)
{
    if (a.length() != b.length())
        return a.length() > b.length();
    return a > b;
}
int main(int argv, char* argc[])
{
    ifstream dat(argc[1]), out(argc[2]);
    int n;
    dat >> n;
    dat.close();
    for (string* i = num; i < num + n; ++i)
        out >> *i;
    out >> oans;
    sort(num, num + n, cmp);
    if (num[0] != oans)
    {
        cerr << "[Failed] Max:" << num[0] << endl;
        cout << "\tOutput: " << oans << endl;
        return -1;
    }
    cerr << "Pass" << endl;
    return 0;
}