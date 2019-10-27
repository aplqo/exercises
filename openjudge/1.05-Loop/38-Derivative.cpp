#include<iostream>
using namespace std;
int main()
{
	int result[100];
	int n, t;
	cin >> n;
	for (int i = n - 1; i >= 0; i--)
	{
		cin >> t;
		result[i] = t * (i + 1);
	}
	if (n == 0)
		cout << 0;
	for (int i = n - 1; i >= 0; i--)
		cout << result[i] << " ";
	return 0;
}
