#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 1000;
constexpr unsigned long long mod = 1000000007;

bool val[maxn + 10];
unsigned int n;
unsigned long long ans;

bool test()
{
    vector<bool> stk;
    for (unsigned int i = 0; i < n; ++i)
    {
        stk.push_back(val[i]);
        if (!stk.empty() && stk.size() >= 3)
        {
            auto t = stk.cend();
            --t;
            if (*t == *(t - 1) && *(t - 1) == *(t - 2))
                for (unsigned int i = 0; i < 3; ++i)
                    stk.pop_back();
        }
    }
    return stk.empty();
}
void fun(const unsigned int pos)
{
    if (pos == n)
    {
        if (test())
        {
            ans = (ans + 1) % mod;
#ifdef Print
            for (unsigned int i = 0; i < n; ++i)
                cout << val[i];
            cout << endl;
#endif
        }
        return;
    }
    val[pos] = false;
    fun(pos + 1);
    val[pos] = true;
    fun(pos + 1);
}

int main(int argc, char* argv[])
{
    if (argc >= 2)
    {
        ifstream in(argv[1]);
        in >> n;
        in.close();
    }
    else
        cin >> n;
    fun(0);
    if (argc >= 2)
    {
        ofstream out(argv[1]);
        out << ans << endl;
        out.close();
    }
    else
        cout << ans << endl;
    return 0;
}