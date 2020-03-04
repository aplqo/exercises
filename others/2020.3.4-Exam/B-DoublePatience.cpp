#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
#include <iostream>
using namespace std;
using num_t = unsigned long long;
using real_t = long double;
const int maxx = 9, maxy = 4;
typedef num_t array_t[4][4][4][4][4][4][4][4][4];

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
        for (unsigned int i = 0; i < 9; ++i)
            ret += 9 - pos[i];
        return ret;
    }
    inline num_t& operator[](array_t dat) const
    {
        return dat[pos[0]][pos[1]][pos[2]][pos[3]][pos[4]][pos[5]][pos[6]][pos[7]][pos[8]];
    }
    unsigned int pos[maxx] = {};
};
array_t suc, all;
unsigned int dat[maxx + 10][maxy + 10];

inline void fun(const state s)
{
    for (unsigned int i = 0; i < 9; ++i)
        for (unsigned int j = i + 1; j < 9; ++j)
        {
            if (dat[s.pos[i]] != dat[s.pos[j]] || s.pos[i] >= 4 || s.pos[j] >= 4)
                continue;
            state r = s.select(i, j);
            if (!r.remain())
            {
                ++s[all];
                ++s[suc];
                continue;
            }
            fun(r);
            s[all] += r[all];
            s[suc] += r[suc];
        }
}
int main()
{
    for (unsigned int i = 0; i < maxx; ++i)
    {
        for (unsigned int j = 0; j < maxy; ++j)
        {
            dat[i][j] = cin.get();
            cin.ignore(2, ' ');
        }
        cin.ignore(1, '\n');
    }
    state ini;
    fun(ini);
    printf("%.6Lf\n", real_t(ini[suc]) / ini[all]);
    return 0;
}
