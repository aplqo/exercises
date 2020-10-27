#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <type_traits>
using namespace std;
const unsigned int maxn = 1e5, maxs = 60, maxc = 10;

struct History
{
    unsigned int time, size;
    inline bool operator<(const History& r) const
    {
        return size != r.size ? size < r.size : time < r.time;
    }
};
struct Node
{
    template <bool inc>
    inline void update(const unsigned int time);

    vector<History> his;
    unsigned int size;
    Node* chl[maxc];
} node[maxn * maxs + 10];
Node root {};

template <bool inc>
inline void Node::update(const unsigned int time)
{
    static conditional_t<inc, plus<unsigned int>, minus<unsigned int>> op;
    size = op(size, 1);
    if (his.empty() || his.back().size < size)
        his.push_back(History { time, size });
}

template <bool inc>
void change(const char str[], const unsigned int tim)
{
    static Node* cur = node;
    const size_t len = strlen(str);
    Node* ptr = &root;
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned int ch = str[i] - 'a';
        if (!ptr->chl[ch])
        {
            new (cur) Node {};
            ptr->chl[ch] = cur++;
        }
        ptr->update<inc>(tim);
        ptr = ptr->chl[ch];
    }
    ptr->update<inc>(tim);
}
unsigned int query(const char str[], const unsigned int v)
{
    const size_t len = strlen(str);
    const Node* ptr = &root;
    for (unsigned int i = 0; i < len && ptr; ++i)
    {
        const unsigned int ch = str[i] - 'a';
        ptr = ptr->chl[ch];
    }
    return !ptr || ptr->his.back().size <= v
        ? 0
        : upper_bound(ptr->his.cbegin(), ptr->his.cend(), History { 0, v + 1 })->time;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, lst = 0;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
    {
        static char str[maxs + 10];
        unsigned int k;
        cin >> k >> str;
        switch (k)
        {
        case 1:
            change<true>(str, i);
            break;
        case 2:
            change<false>(str, i);
            break;
        case 3:
        {
            unsigned long long a, b, c;
            cin >> a >> b >> c;
            const unsigned int v = (a * lst + b) % c;
            const unsigned int res = query(str, v);
            if (res)
                cout << res << "\n";
            else
                cout << "-1\n";
            lst = res ? res : 1;
            break;
        }
        }
    }
    return 0;
}