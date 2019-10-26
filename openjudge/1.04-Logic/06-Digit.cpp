#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int a;
	cin >> a;
	if ((a >= 10) && (a < 100))
		cout << 1;
	else
		cout << 0;
	return 0;
}
