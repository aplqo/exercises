using System;
using System.Numerics;

namespace _1737_ConnectedGraph
{
    class Program
    {
        static BigInteger[] pow, fac;
        static void GetPow(uint n)
        {
            pow = new BigInteger[n + 1];
            pow[0] = 1;
            for (uint i = 1; i <= n; ++i)
                pow[i] = pow[i - 1] * 2;
        }
        static void GetFactor(uint n)
        {
            fac = new BigInteger[n + 1];
            fac[0] = 1;
            for (uint i = 1; i <= n; ++i)
                fac[i] = fac[i - 1] * i;
        }
        static BigInteger C(uint n, uint m)
        {
            return fac[n] / fac[n - m] / fac[m];
        }
        static BigInteger Dp(uint n)
        {
            BigInteger[] f = new BigInteger[n + 1];
            f[1] = 1;
            for (uint j = 2; j <= n; ++j)
            {
                f[j] = pow[j * (j - 1) / 2];
                for (uint k = 1; k < j; ++k)
                    f[j] -= f[k] * C(j - 1, k - 1) * pow[(j - k) * (j - k - 1) / 2];
            }
            return f[n];
        }
        static void Main(string[] args)
        {
            uint n = uint.Parse(Console.ReadLine());
            GetPow(n * (n - 1) / 2);
            GetFactor(n);
            Console.WriteLine(Dp(n));
        }
    }
}
