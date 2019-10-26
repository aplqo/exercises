#include<iostream>
using std::cin;
using std::cout;
int main()
{
	unsigned int x;
	cin >> x;
	if (x % 2)
	{
		cout << "odd";
	}
	else
	{
		cout << "even";
	}
	return 0;
}
