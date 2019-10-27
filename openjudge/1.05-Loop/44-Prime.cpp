#include<iostream>
using namespace std;
int main()
{
	int n, ans = 0;
	bool num[104730] = { true,true };
	cin >> n;
	int i;
	for (i = 2; i < 104730 && ans < n; i++)
	{
		if (num[i]) continue;
		for (int j = 2; j <= 104729 / i; j++)
			num[i * j] = true;
		ans++;
	}
	cout << i - 1;
	return 0;
}
