#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
#include <queue>
#include <valarray>
using namespace std;
const unsigned int maxm = 10, maxl = 10;
const unsigned long long mod = 100000;
const unsigned int tab[] = { 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0 };

unsigned int cnt;
typedef valarray<unsigned long long> Vector;
struct Matrix
{
    Matrix()
    {
        for (unsigned int i = 0; i <= cnt; ++i)
            val[i].resize(cnt + 1);
    }
    void unit()
    {
        for (unsigned int i = 0; i <= cnt; ++i)
            val[i][i] = 1;
    }
    inline const Vector& operator[](const unsigned int i) const { return val[i]; }
    inline Vector& operator[](const unsigned int i) { return val[i]; }
    Vector val[maxl * maxm + 1];
};
inline Vector operator*(const Matrix& m, const Vector& v)
{
    Vector ret(cnt + 1);
    for (unsigned int i = 0; i <= cnt; ++i)
        ret = (ret + (m[i] * v[i]) % mod) % mod;
    return ret;
}
inline Matrix operator*(const Matrix& l, const Matrix& r)
{
    Matrix ret;
    for (unsigned int i = 0; i <= cnt; ++i)
        ret[i] = l * r[i];
    return ret;
}
inline Matrix operator^(const Matrix& l, unsigned int e)
{
    Matrix ret, t = l;
    ret.unit();
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = ret * t;
        t = t * t;
    }
    return ret;
}
unsigned int ch[maxm * maxl + 10][4], fail[maxm * maxl + 10];
bool b[maxm * maxl + 10];

void add(const char* str)
{
    const size_t len = strlen(str);
    unsigned int p = 0;
    for (unsigned int i = 0; i < len; ++i)
    {
        const unsigned int c = tab[str[i] - 'A'];
        if (!ch[p][c])
            ch[p][c] = ++cnt;
        p = ch[p][c];
    }
    b[p] = true;
}
Matrix make()
{
    Matrix ret;
    queue<unsigned int> q;
    for (unsigned int i = 0; i < 4; ++i)
    {
        if (ch[0][i])
            q.push(ch[0][i]);
        if (!b[ch[0][i]])
            ++ret[0][ch[0][i]];
    }
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (unsigned int i = 0; i < 4; ++i)
        {
            if (!ch[cur][i])
                ch[cur][i] = ch[fail[cur]][i];
            else
            {
                fail[ch[cur][i]] = ch[fail[cur]][i];
                b[ch[cur][i]] |= b[ch[fail[cur]][i]];
                q.push(ch[cur][i]);
            }
            if (!b[ch[cur][i]] && !b[cur])
                ++ret[cur][ch[cur][i]];
        }
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (cin)
    {
        unsigned int n, m;
        cin >> m >> n;
        if (cin.fail())
            break;
        for (unsigned int i = 0; i < m; ++i)
        {
            static char str[maxl + 10];
            cin >> str;
            add(str);
        }
        const Matrix r = make() ^ n;
        unsigned long long ret = 0;
        for (unsigned int i = 0; i <= cnt; ++i)
            ret = (ret + r[0][i]) % mod;
        cout << ret << endl;
        cnt = 0;
        memset(ch, 0, sizeof(ch));
        memset(fail, 0, sizeof(fail));
        memset(b, 0, sizeof(b));
    }
    return 0;
}