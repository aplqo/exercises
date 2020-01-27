#include <fstream>
#include <iostream>
#include <map>
#include <utility>
using namespace std;
typedef int num;
const int maxn = 5e3 + 5;

map<pair<int, int>, num> equ;
ifstream in, out, ans;
num res[maxn];
int n, m;

int main(int argc, char* argv[])
{
    in.open(argv[1]);
    out.open(argv[2]);
    ans.open(argv[3]);
    in >> n >> m;
    for (int i = 0; i < m; ++i)
    {
        int c1, c2;
        num n;
        in >> c1 >> c2 >> n;
        equ[make_pair(c1, c2)] = n;
    }
    if (ans.peek() == 'N' && out.peek() == 'N')
        goto suc;
    else if (out.peek() == 'N')
    {
        cout << "Except an result,bur no found" << endl;
        return 1;
    }
    ans.close();
    for (int i = 1; i <= n; ++i)
        out >> res[i];
    for (auto& i : equ)
        if (res[i.first.first] - res[i.first.second] > i.second)
        {
            cout << "Test filed on x1=" << i.first.first << " x2=" << i.first.second << endl;
            return 1;
        }
suc:;
    cout << "Test passed." << endl;
    return 0;
}