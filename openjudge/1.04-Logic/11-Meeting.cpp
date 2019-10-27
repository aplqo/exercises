#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int a;
	cin >> a;
	if ((a % 2) && (a != 7))
		cout << "NO";
	else
		cout << "YES";
	return 0;
}
