#include <iostream>
using std::cin;
using std::cout;

int main()
{
	unsigned int apples[10], heigh;
	unsigned int num = 0;
	for (auto& i : apples)
	{
		cin >> i;
	}
	cin >> heigh;
	heigh += 30;
	for (const auto i : apples)
	{
		if (i <= heigh)
		{
			num++;
		}
	}
	cout << num;
	return 0;
}
