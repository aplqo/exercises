#include<iostream>
using namespace std;
int main()
{
	int l, r,t,ans=0;
	cin >> l >> r;
	for (int i = l; i <= r; i++)
	{
		t = i;
		while (t)
		{
			if (t % 10 == 2)
				ans++;
			t /= 10;
		}
	}
	cout << ans;
	return 0;
}
