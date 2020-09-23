#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
#include <algorithm>
using namespace std;
const unsigned int maxl = 32, maxn = 100;
constexpr unsigned long long mod = 998244353;

class Number
{
public:
	Number() = default;
	Number(const unsigned long long v) 
		: val(v % mod) {};
#define decl(x) \
	friend inline Number operator x(const Number l,const Number r){return (l.val + mod) x r.val;} \
   	inline Number& operator x##=(const Number r) { return *this = *this x r; }
	decl(+);
	decl(-);
	decl(*);
#undef decl
	friend inline ostream& operator<<(ostream& os, const Number v)
	{
		return os << v.val;
	}

private:
	unsigned long long val = 0;
};

Number count(const unsigned int n, const unsigned int a)
{
	static Number f[maxn * maxl + 10] = { 1 }, sum[maxn * maxl + 10];
	fill(f + 1, f + 1 + n * a, 0);
	fill(sum, sum + 1 + n * a, 1);
	for(unsigned int i = 1; i <= n * 2; ++i)
	{
		for(unsigned int j = 1; j <= n * a; ++j)
			f[j] = sum[j] - (j > a? sum[j - a - 1] : 0);
		partial_sum(f, f + 1 + n * a, sum);
	}
	return f[n * a];
}
Number quickPow(Number a, unsigned long long e)
{
	Number ret = 1;
	for(; e; e >>= 1)
	{
		if(e & 0x01)
			ret *= a;
		a *= a;
	}
	return ret;
}

Number factor(const unsigned int n, const unsigned int m)
{
	Number ret = 1;
	unsigned int cnt = 0, acc = n;
	for(unsigned long long i = 2; i * i <= n; ++i)
	{
		if(n % i)
			continue;
		++cnt;
		if(i * i != n)
			++cnt;
		if(acc % i)
			continue;
		unsigned int ex = 0;
		for(; (acc % i) == 0; acc /= i)
			++ex;
		ret *= count(m, ex);
	}
	if(acc > 1)
		ret *= count(m, 1);
	return (quickPow(cnt + 2, m * 2) - ret) * quickPow(2, mod - 2) + ret;
}
int main()
{
#ifndef APTEST
	ios_base::sync_with_stdio(false);
#endif
	unsigned int n, m;
	cin >> n >> m;
	cout << factor(n, m) << "\n";
	return 0;
}
