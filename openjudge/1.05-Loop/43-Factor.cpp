#include<iostream>
#include<cmath>
using namespace std;
int main()
{
	int n, ans = 0;
	cin >> n;
	for (int i = sqrt(n); i > 1; i--)
		if (n % i == 0)
		{
			cout << n / i;
			return 0;
		}
}
