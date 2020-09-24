#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <valarray>
using namespace std;
const unsigned int col = 6;

unsigned long long n;
typedef valarray<unsigned long long> Vector;
class Matrix
{
public:
    inline void init()
    {
        for (unsigned int i = 0; i < col; ++i)
            val[i].resize(col);
    }
    inline void unit()
    {
        init();
        for (unsigned int i = 0; i < col; ++i)
            val[i][i] = 1;
    }
    inline const Vector& operator[](const unsigned int v) const { return val[v]; }
    inline Vector& operator[](const unsigned int v) { return val[v]; }
    inline Vector operator*(const Vector& v) const
    {
        Vector ret;
        ret.resize(col);
        for (unsigned int i = 0; i < col; ++i)
            ret = (ret + val[i] * v[i]) % n;
        return ret;
    }
    inline Matrix operator*(const Matrix& r) const
    {
        Matrix ret;
        ret.init();
        for (unsigned int i = 0; i < col; ++i)
            ret[i] = (*this * r[i] + ret[i]) % n;
        return ret;
    }
    inline Matrix operator^(unsigned long long e) const
    {
        Matrix ret, a = *this;
        ret.unit();
        for (; e; e >>= 1)
        {
            if (e & 0x01)
                ret = ret * a;
            a = a * a;
        }
        return ret;
    }

    Vector val[col];
};
const Matrix mat {
    Vector { 2, 1, 1, 1, 0, 0 },
    Vector { 1, 2, 1, 1, 0, 0 },
    Vector { 1, 0, 1, 0, 0, 0 },
    Vector { 0, 1, 0, 1, 0, 0 },
    Vector { 1, 1, 1, 1, 1, 0 },
    Vector { 2, 2, 2, 2, 1, 1 }
}; // {x, y, dx, dy, step, 1}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    const static auto toPostive = [](const int v) -> unsigned long long {
        const long long t = n;
        return (v % t + t) % t;
    };
    unsigned int sx, sy;
    unsigned long long t;
    int dx, dy;
    cin >> n >> sx >> sy >> dx >> dy >> t;
    const Vector endV = (mat ^ t) * Vector { sx - 1, sy - 1, toPostive(dx), toPostive(dy), 0, 1 };
    cout << endV[0] + 1 << " " << endV[1] + 1 << "\n";
    return 0;
}