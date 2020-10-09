#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxb = 1e3;

unsigned int blockSize;
int dat[maxn + 10];
class Block
{
public:
    void build(int* l, int* r)
    {
        this->l = l;
        this->r = r;
    }
    unsigned int countAll(const int c)
    {
        unsigned int ret = 0;
        if (!same)
            ret = count(l, r, c);
        else
            ret = c == laz ? r - l : 0;
        same = true;
        laz = c;
        return ret;
    }
    unsigned int countPart(int* l, int* r, const int c)
    {
        unsigned int ret = 0;
        if (same)
        {
            fill(this->l, this->r, laz);
            ret = c == laz ? r - l : 0;
            same = false;
        }
        else
            ret = count(l, r, c);
        fill(l, r, c);
        return ret;
    }

private:
    bool same = false;
    int *l, *r, laz;
} blk[maxb + 10];

static void init(const unsigned int n)
{
    for (unsigned int i = 0; i <= n / blockSize; ++i)
        blk[i].build(dat + blockSize * i, dat + min(n, blockSize * (i + 1)));
}
unsigned int count(const unsigned int l, const unsigned int r, const int c)
{
    unsigned int ret = 0;
    const unsigned int indl = (l + blockSize - 1) / blockSize, indr = r / blockSize;
    if (l / blockSize != r / blockSize)
    {
        ret += blk[l / blockSize].countPart(dat + l, dat + indl * blockSize, c)
            + blk[indr].countPart(dat + indr * blockSize, dat + r, c);
        for (unsigned int i = indl; i < indr; ++i)
            ret += blk[i].countAll(c);
    }
    else
        ret = blk[l / blockSize].countPart(dat + l, dat + r, c);
    return ret;
}

int main(int, char* argv[])
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<int>(cin), n, dat);
    blockSize = sqrt(n);
    init(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int l, r;
        int c;
        cin >> l >> r >> c;
        cout << count(l - 1, r, c) << "\n";
    }
    return 0;
}