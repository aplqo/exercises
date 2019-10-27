#include<iostream>
using namespace std;
int main()
{
	int n, ans = 0, tmp;
	char t[4];
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin.ignore(1);
		cin.read(t, 4);
		tmp = t[3] - t[2] - t[1] - t[0] + 2 * '0';
		if (tmp > 0)
			ans++;
	}
	cout << ans;
	return 0;
}

