#include<iostream>
#include<algorithm>
using namespace std;
int main()
{
	int m, n;
	bool lamps[5000] = { true };
	cin >> m >> n;
	for (int i = 2; i <= n; i++)
	{
		for (int j = 1; j <= m / i; j++)
			lamps[i * j] = !lamps[i * j];
	}
	bool t = false;
	for (int i = 1; i <= m; i++)
	{
		if (!lamps[i])
		{
			if (t) cout << ',';
			cout << i;
			t = true;
		}
	}
	return 0;
}

