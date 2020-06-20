#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
#include <set>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxv = 1e7;

mt19937_64 rnd;
multiset<int> s;

int setGen()
{
    auto it = s.begin();
    for (unsigned int i = GetRandom<size_t>(0, s.size() - 1); i; --i)
        ++it;
    return *it;
}
void createOperate(ofstream& f, const unsigned int n, const int ma)
{
    uniform_int_distribution<int> disX(-ma, ma);
    uniform_int_distribution<unsigned int> diso(1, 6);
    {
        const int t = disX(rnd);
        f << 1 << " " << t << endl;
        s.insert(t);
    }

    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int op = diso(rnd);
        int x;
        if (((op == 5 || op == 6) && s.size() < 2) || (op != 1 && !s.size()))
        {
            --i;
            continue;
        }
        f << op << " ";
        switch (op)
        {
        case 1:
            x = disX(rnd);
            s.insert(x);
            f << x << endl;
            break;
        case 2:
            x = setGen();
            s.erase(x);
            f << x << endl;
            break;
        case 3:
            f << setGen() << endl;
            break;
        case 4:
            f << GetRandom<size_t>(1, s.size(), rnd) << endl;
            break;
        case 5:
            do
            {
                x = disX(rnd);
            } while (s.lower_bound(x) == s.begin() || s.lower_bound(x) == s.end());
            f << x << endl;
            break;
        case 6:
            do
            {
                x = disX(rnd);
            } while (s.upper_bound(x) == s.end());
            f << x << endl;
            break;
        }
    }
}
int main(int argc, char* argv[])
{
    Init();
    Init(rnd);
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    createOperate(in, n, MayBe(atoi(argv[2]), maxv));
    return 0;
}