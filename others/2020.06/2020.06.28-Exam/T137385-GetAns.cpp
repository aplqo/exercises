#include <fstream>
using namespace std;
const unsigned int maxn = 1e5;

class tree
{
public:
    void build(const unsigned int l, const unsigned int r, tree*(&cur), ifstream& in)
    {
        lv = l;
        rv = r;
        if (l == r)
        {
            lc = rc = nullptr;
            return;
        }
        in >> mv;
        lc = ++cur;
        lc->build(l, mv, cur, in);
        rc = ++cur;
        rc->build(mv + 1, r, cur, in);
    }
    unsigned int find(const unsigned int l, const unsigned int r) const
    {
        if (l == lv && r == rv)
            return 1;
        if (r <= mv)
            return lc->find(l, r);
        else if (l > mv)
            return rc->find(l, r);
        else
            return lc->find(l, mv) + rc->find(mv + 1, r);
    }

private:
    unsigned int lv, mv, rv;
    tree *lc, *rc;
} nodes[maxn * 4], *root = nodes;

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, m;
    in >> n >> m;
    {
        tree* c = nodes;
        root->build(1, n, c, in);
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        in >> l >> r;
        ans << root->find(l, r) << endl;
    }
    return 0;
}