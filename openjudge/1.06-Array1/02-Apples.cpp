#include<iostream>
using namespace std;
int main()
{
	int apples[10], h, ans = 0;
	for (int i = 0; i < 10; i++)
		cin >> apples[i];
	cin >> h;
	h += 30;
	for (int i = 0; i < 10; i++)
		if (apples[i] <= h)
			ans++;
	cout << ans;
	return 0;
}

