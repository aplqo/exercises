#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1000000;

struct vertex;
struct edge
{
    inline void Init(unsigned int from, unsigned int to, bool typ);
    inline void Inverse();
    vertex* to;
    bool color;
    edge *pre = nullptr, *rev = nullptr;
} ed[maxn + 10];
struct vertex
{
    edge* head = nullptr;
    unsigned int degree = 0;
} ve[maxn + 10];
unsigned int n, m;

inline void edge::Inverse()
{
    if (color)
        --(to->degree);
    else
        ++(to->degree);
    color = !color;
}
inline void edge::Init(unsigned int from, unsigned int to, bool typ)
{
    this->to = ve + to;
    this->color = typ;
    pre = ve[from].head;
    ve[from].head = this;
    if (typ)
        ++ve[to].degree;
}