#include<iostream>
using namespace std;
int main()
{
	int n, ans = 0, t;
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		t = i;
		while (t)
		{
			if (t % 10 == 1)
				ans++;
			t /= 10;
		}
	}
	cout << ans;
	return 0;
}
