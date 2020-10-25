#include <algorithm>
#include <fstream>
#include <numeric>
#include <vector>
using namespace std;

vector<long long> vec;

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n = 0, pos = 0;
    in >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int l, r;
        long long x;
        char op;
        in >> op;
        switch (op)
        {
        case 'I':
            in >> x;
            vec.insert(vec.begin() + pos, x);
            ++pos;
            break;
        case 'D':
            vec.erase(vec.begin() + pos - 1);
            --pos;
            break;
        case 'L':
            if (pos)
                --pos;
            break;
        case 'R':
            pos = min<unsigned int>(pos + 1, vec.size());
            break;
        case 'Q':
            in >> l >> r;
            ans << accumulate(vec.cbegin() + l - 1, vec.cbegin() + r, 0ll) << "\n";
            break;
        case 'C':
            in >> l >> x;
            vec[l - 1] = x;
            break;
        }
    }
    return 0;
}
