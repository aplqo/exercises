#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn = 1005;

struct peo
{
	int t, n;
}p[maxn];
unsigned long long ans, lst;
int main()
{
	unsigned int n;
	cin >> n;
	for (peo* i = p; i < p + n; i++)
	{
		cin >> i->t;
		i->n = i - p + 1;
	}
	sort(p, p + n, [](const peo& a, const peo& b)->
		bool {return a.t != b.t ? a.t < b.t : a.n < b.n; });
	for_each(p, p + n, [](peo& i)->void {cout << i.n << " "; });
	cout << endl;
	for (peo* i = p; i < p + n; i++)
	{
		ans += lst;
		lst += i->t;
	}
	double avr = (double)ans / n;
	printf("%.2lf", avr);
	return 0;
}