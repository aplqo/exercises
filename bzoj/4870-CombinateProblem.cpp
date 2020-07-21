#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e9, maxk = 50;

unsigned long long mod;
unsigned int k;
struct Vector
{
    inline unsigned long long& operator[](const size_t ind) { return val[ind]; }
    inline const unsigned long long operator[](const size_t ind) const { return val[ind]; }
    unsigned long long val[maxk + 1] = {};
};
struct Matrix
{
    inline Vector& operator[](const size_t ind) { return val[ind]; }
    inline const Vector& operator[](const size_t ind) const { return val[ind]; }
    void unit()
    {
        for (unsigned int i = 0; i < k; ++i)
            val[i][i] = 1;
    }

    Vector val[maxk + 1];
};
Vector operator*(const Matrix& m, const Vector& v)
{
    Vector ret;
    for (unsigned int i = 0; i < k; ++i)
        for (unsigned int j = 0; j < k; ++j)
            ret[j] = (ret[j] + (m[i][j] * v[i]) % mod) % mod;
    return ret;
}
Matrix operator*(const Matrix& m, const Matrix& r)
{
    Matrix ret;
    for (unsigned int i = 0; i < k; ++i)
        ret[i] = m * r[i];
    return ret;
}
Matrix operator^(Matrix m, unsigned long long e)
{
    Matrix ret;
    ret.unit();
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret = ret * m;
            e ^= i;
        }
        m = m * m;
    }
    return ret;
}

Vector buildIV(const unsigned int k)
{
    Vector ret;
    ret[0] = 1;
    return ret;
}
Matrix buildMatrix(const unsigned int k)
{
    Matrix ret;
    ret[0][0] = 1;
    ++ret[k - 1][0];
    for (unsigned int i = 1; i < k; ++i)
        ret[i][i] = ret[i - 1][i] = 1;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n, r;
    cin >> n >> mod >> k >> r;
    cout << ((buildMatrix(k) ^ (n * k)) * buildIV(k))[r] << endl;
    return 0;
}