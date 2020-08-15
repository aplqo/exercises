using System;
using System.Numerics;

namespace virus
{
    class Program
    {
        static System.Numerics.BigInteger fib(uint n)
        {
            BigInteger[] f = new BigInteger[n + 1];
            f[1] = 1;
            f[2] = 3;
            for (uint i = 3; i <= n; ++i)
                f[i] = f[i - 1] + f[i - 2];
            return f[n] * f[n] - (n % 2 == 1 ? 0 : 4);
        }
        static void Main(string[] args)
        {
            uint n=uint.Parse(Console.ReadLine());
            Console.WriteLine(fib(n));
        }
    }
}