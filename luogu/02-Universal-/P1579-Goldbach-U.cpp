#include<iostream>
#include<cmath>
#include<cstdlib>
using namespace std;
int primes[20001], num;
bool p[20001];
int ans[3];

void getPrimes(int n)
{
	for (int i = 2; i <= sqrt(n); i++)
	{
		if (p[i]) continue;
		for (bool* j = p + i * 2; j <= p + n; j += i)
			* j = true;
	}
	for (int i = 2; i < n; i++)
		if (!p[i])
		{
			primes[num] = i;
			num++;
		}
}
template<int dep>
void fun(int n)
{
	for (int i = 0; i < num; i++)
	{
		ans[dep] = primes[i];
		fun<dep + 1>(n - primes[i]);
	}
}
template<>
void fun<3>(int n)
{
	if (n == 0)
	{
		for (auto i : ans)
			cout << i << ' ';
		exit(0);
	}
}
int main()
{
	int a;
	cin >> a;
	getPrimes(a);
	fun<0>(a);
	return 0;
}
