#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5;

vector<int> d;

int main(int argc, char* argv[])
{
    ifstream fin(argv[1]);
    ofstream fans(argv[2]);
    unsigned int n;
    fin >> n;
    d.reserve(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        int opt, x;
        fin >> opt >> x;
        switch (opt)
        {
        case 1:
            d.insert(lower_bound(d.begin(), d.end(), x), x);
            break;
        case 2:
            d.erase(lower_bound(d.begin(), d.end(), x));
            break;
        case 3:
            fans << (lower_bound(d.begin(), d.end(), x) - d.begin() + 1) << endl;
            break;
        case 4:
            fans << d[x - 1] << endl;
            break;
        case 5:
            fans << *(lower_bound(d.begin(), d.end(), x) - 1) << endl;
            break;
        case 6:
            fans << *(upper_bound(d.begin(), d.end(), x)) << endl;
            break;
        }
    }
    return 0;
}