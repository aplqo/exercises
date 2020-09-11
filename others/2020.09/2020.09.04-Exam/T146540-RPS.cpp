#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxl = 20;

enum Type
{
    P = 0,
    R = 1,
    S = 2
};
const Type win[] = { R, S, P };
const char letter[] = { 'P', 'R', 'S' };

char str[(1 << maxl) + 10];
unsigned int cnt[3];
unsigned int Log2(const unsigned int v)
{
    unsigned int i = 0, t = 1;
    while (v != t)
    {
        ++i;
        t <<= 1;
    }
    return i;
}
bool buildDfs(const Type root, const unsigned int dep, char* str)
{
    if (!dep)
    {
        if (!cnt[root])
            return false;
        --cnt[root];
        *str = letter[root];
        return true;
    }
    return buildDfs(root, dep - 1, str) && buildDfs(win[root], dep - 1, str + (1 << (dep - 1)));
}
void SortStr(unsigned int l, unsigned int r, char* str)
{
    if (r - l == 1)
        return;
    const unsigned int mid = (l + r) >> 1;
    SortStr(l, mid, str);
    SortStr(mid, r, str);
    if (!lexicographical_compare(str + l, str + mid, str + mid, str + r))
        swap_ranges(str + l, str + mid, str + mid);
}
bool build(const Type root, const unsigned int r, const unsigned int p, const unsigned int s)
{
    static char buf[(1 << maxl) + 1];
    cnt[R] = r;
    cnt[P] = p;
    cnt[S] = s;
    const unsigned int sum = p + r + s;
    if (!buildDfs(root, Log2(r + p + s), buf))
        return false;
    SortStr(0, r + p + s, buf);
    if (lexicographical_compare(buf, buf + sum, str, str + sum))
        swap_ranges(buf, buf + sum, str);
    return true;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int r, p, s;
    cin >> r >> p >> s;
    fill(str, str + r + s + p, 'z');
    if (build(P, r, p, s) || build(R, r, p, s) || build(S, r, p, s))
        cout << str << "\n";
    else
        cout << "IMPOSSIBLE\n";
    return 0;
}