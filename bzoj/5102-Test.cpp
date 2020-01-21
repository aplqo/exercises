#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
using namespace std;
const int maxn = 10000010;

struct range
{
    int l, r;
} r[maxn];
int n, k, res;
int ret = 0;
ifstream in, out, ans;

inline void read()
{
    in >> n >> k;
    for (range* i = r + 1; i < r + n + 1; ++i)
        in >> i->l >> i->r;
    in.close();
    ans >> res;
    ans.close();
}
int main(int argc, char* argv[])
{
    in.open(argv[1]);
    out.open(argv[2]);
    ans.open(argv[3]);
    read();

    int output, rl = 0, rr = INT_MAX;
    out >> output;
    for (int i = 0; i < k; ++i)
    {
        int id;
        out >> id;
        rl = max(r[id].l, rl);
        rr = min(r[id].r, rr);
    }
    if (output != res)
    {
        cerr << "Program output " << output << " different from answer " << res << endl;
        ret = 1;
    }
    if (rr - rl != output)
    {
        cerr << "Program output incorrect" << endl;
        ret += 2;
    }
    return ret;
}