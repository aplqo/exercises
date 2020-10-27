#ifdef APTEST
#include "debug_tools/program.h"
#endif
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iomanip>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 5, maxl = 15, maxv = maxn * maxl, maxa = 26;
typedef double real;

struct Matrix
{
    Matrix operator*(const Matrix& r) const;
    void unit(const unsigned int s);
    inline const real* operator[](const unsigned int i) const { return val[i]; }
    inline real* operator[](const unsigned int i) { return val[i]; }

    real val[maxv + 1][maxv + 1];
};
unsigned int ch[maxv + 10][maxa + 10], fail[maxv + 10], cur;
bool ed[maxv + 10];

Matrix Matrix::operator*(const Matrix& r) const
{
    Matrix ret {};
    for (unsigned int i = 0; i <= cur + 1; ++i)
        for (unsigned int j = 0; j <= cur + 1; ++j)
            for (unsigned int k = 0; k <= cur + 1; ++k)
                ret[i][k] += r[i][j] * val[j][k];
    return ret;
}
void Matrix::unit(const unsigned int s)
{
    for (unsigned int i = 0; i <= s; ++i)
        val[i][i] = 1;
}
Matrix quickPow(Matrix m, unsigned int e)
{
    Matrix ret {};
    ret.unit(cur + 1);
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = ret * m;
        m = m * m;
    }
    return ret;
}

void insert(const char str[])
{
    const size_t l = strlen(str);
    unsigned int pos = 0;
    for (unsigned int i = 0; i < l; ++i)
    {
        const unsigned int c = str[i] - 'a';
        if (!ch[pos][c])
            ch[pos][c] = ++cur;
        pos = ch[pos][c];
    }
    ed[pos] = true;
}
void buildAho(const unsigned int a)
{
    queue<unsigned int> q;
    for (unsigned int i = 0; i < a; ++i)
        if (ch[0][i])
            q.push(ch[0][i]);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        ed[cur] |= ed[fail[cur]];
        for (unsigned int i = 0; i < a; ++i)
            if (!ch[cur][i])
                ch[cur][i] = ch[fail[cur]][i];
            else
            {
                fail[ch[cur][i]] = ch[fail[cur]][i];
                q.push(ch[cur][i]);
            }
    }
}
Matrix buildMatrix(const unsigned int siz, const unsigned int a)
{
    const real p = real(1) / a;
    Matrix ret {};
    for (unsigned int i = 0; i <= siz; ++i)
        for (unsigned int j = 0; j < a; ++j)
        {
            const unsigned int to = ch[i][j];
            if (ed[to])
            {
                ret[i][0] += p;
                ret[i][siz + 1] += p;
            }
            else
                ret[i][to] += p;
        }
    ret[siz + 1][siz + 1] = 1;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, l, a;
    cin >> n >> l >> a;
    for (unsigned int i = 0; i < n; ++i)
    {
        static char str[maxl + 10];
        cin >> str;
        insert(str);
    }
    buildAho(a);
    cout << fixed << setprecision(7) << quickPow(buildMatrix(cur, a), l)[0][cur + 1] << "\n";
    return 0;
}