#include <iostream>
using namespace std;
int main()
{
	char sum[14];
	int t1 = 0, rat = 1;
	cin.read(sum, 13);
	for (char* i = sum; i < sum + 11; i++)
		if (*i != '-')
		{
			t1 += (*i - '0') * rat;
			rat++;
		}
	if ((t1 % 11 + '0' == sum[12]) || (t1 % 11 == 10 && sum[12] == 'X'))
	{
		cout << "Right";
	}
	else
	{
		cout.write(sum, 12);
		if (t1 % 11 != 10)
			cout << t1 % 11;
		else
			cout << 'X';
	}
	return 0;
}
