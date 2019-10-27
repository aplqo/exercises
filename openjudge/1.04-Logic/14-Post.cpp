#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int d, ans=8;
	char x;
	cin >> d >> x;
	if (d > 1000)
	{
		int r = (d-1000) / 500;
		if ((d - 1000) % 500) r++;
		ans += r * 4;
	}
	if (x == 'y') ans += 5;
	cout << ans;
	return 0;
}

