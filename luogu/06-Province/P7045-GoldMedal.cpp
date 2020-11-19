#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 5e4;

inline bool isDifferent(const unsigned int x, const unsigned int y)
{
    cout << x << " " << y << endl;
    unsigned int ret;
    cin >> ret;
    return ret;
}
void solve()
{
    vector<unsigned int> ans, seq;
    queue<unsigned int> lst;
    unsigned int n, q;
    cin >> n >> q;
    seq.push_back(0);
    for (unsigned int i = 1; i < n; ++i)
    {
        if (lst.empty())
        {
            if (isDifferent(i, seq.back()))
                seq.push_back(i);
            else
                lst.push(i);
        }
        else
        {
            if (isDifferent(i, lst.front()))
            {
                seq.push_back(i);
                seq.push_back(lst.front());
                lst.pop();
            }
            else
                lst.push(i);
        }
    }
    ans.reserve(n);
    bool ldif = true;
    for (auto it = seq.cbegin(); true; ++it)
    {
        const bool cdif = lst.empty() || it == seq.cend() || isDifferent(*it, lst.front());
        if (!lst.empty() && cdif && ldif)
        {
            ans.push_back(lst.front());
            lst.pop();
        }
        if (it == seq.cend())
            break;
        else
        {
            ldif = cdif;
            ans.push_back(*it);
        }
    }
    if (!lst.empty())
        cout << "-1\n";
    else
    {
        cout << n << "\n";
        copy(ans.cbegin(), ans.cend(), ostream_iterator<unsigned int>(cout, " "));
    }
    cout << endl;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}