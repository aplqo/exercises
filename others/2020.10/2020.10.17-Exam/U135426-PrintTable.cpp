#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
using namespace std;

unsigned long long msk;
map<unsigned long long, unsigned long long> mem;

#define lowbit(x) ((x) & -(x))
unsigned long long dfs(const unsigned long long stat)
{
    if (auto it = mem.find(stat); it != mem.end())
        return it->second;
    set<unsigned long long> chl;
    for (unsigned int j = (~stat) & msk; j; j -= lowbit(j))
    {
        const unsigned long long lb = lowbit(j);
        if ((stat & lb) || (stat & (lb << 1)) || (stat & (lb >> 1)))
            continue;
        chl.insert(dfs(stat | lb));
    }
    unsigned long long acc = 0;
    for (unsigned long long i : chl)
    {
        if (i > acc)
            break;
        acc = i + 1;
    }
    return mem[stat] = acc;
}
unsigned long long sg(const unsigned int n)
{
    mem.clear();
    msk = (1ull << n) - 1;
    return dfs(0);
}
void printSG(const unsigned int n)
{
    for (auto it : mem)
    {
        clog.put('\t');
        for (unsigned int j = 0; j < n; ++j)
            clog.put(it.first & (1 << j) ? '1' : '0');
        clog << ": " << it.second << "\n";
    }
    clog << flush;
}

int main(int, char* argv[])
{
    while (true)
    {
        unsigned int l, r;
        cout << "chess>" << flush;
        cin >> l >> r;
        if (cin.fail())
            break;
        const unsigned long long w = log10(r);
        for (unsigned int i = l; i <= r; ++i)
        {
            cout << setw(w + 1) << i << ": " << sg(i) << "\n";
#ifdef LogSub
            printSG(i);
#endif
        }
    }
    return 0;
}