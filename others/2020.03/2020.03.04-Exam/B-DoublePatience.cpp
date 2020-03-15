#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
using real_t = long double;
template <class T>
using array_t = T[5][5][5][5][5][5][5][5][5];

struct state
{
    state select(const unsigned int p1, const unsigned int p2) const
    {
        state ret = *this;
        ++ret.pos[p1];
        ++ret.pos[p2];
        return ret;
    }
    unsigned int remain() const
    {
        unsigned int ret = 0;
        for (auto i : pos)
            ret += 4 - i;
        return ret;
    }
    template <class T>
    inline T& operator[](array_t<T> dat) const
    {
        return dat[pos[0]][pos[1]][pos[2]][pos[3]][pos[4]][pos[5]][pos[6]][pos[7]][pos[8]];
    }
    unsigned int pos[9] = {};
};
array_t<real_t> f;
array_t<bool> vis;
unsigned int dat[10][5];

real_t fun(const state&& s)
{
    if (!s.remain())
        return s[f] = 1;
    if (s[vis])
        return s[f];
    s[vis] = true;
    unsigned int p = 0;
    for (unsigned int i = 0; i < 9; ++i)
    {
        if (s.pos[i] > 3)
            continue;
        for (unsigned int j = i + 1; j < 9; ++j)
        {
            if (s.pos[j] > 3 || dat[i][s.pos[i]] != dat[j][s.pos[j]])
                continue;
            ++p;
            s[f] += fun(s.select(i, j));
        }
    }
    s[f] = p ? s[f] / real_t(p) : 0;
    return s[f];
}
int main()
{
    for (unsigned int i = 0; i < 9; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
        {
            dat[i][3 - j] = cin.get();
            cin.ignore(2, ' ');
        }
        dat[i][0] = cin.get();
        cin.ignore(2, '\n');
    }
    fun(state());
    printf("%.6Lf\n", state()[f]);
    return 0;
}
