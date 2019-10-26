#include <iostream>
#include <string>
#include <valarray>
using namespace std;
int main()
{
	valarray<int> ans(26);
	for (int i = 0; i < 4; i++)
	{
		string s;
		getline(cin, s);
		for (auto i : s)
			if (isupper(i))
				ans[i - 'A']++;
	}
	for (int i = ans.max(); i > 0; i--)
	{
		for (int j = 0; j < 25; j++)
		{
			if (ans[j] >= i)
				cout << '*';
			else
				cout << ' ';
			cout << ' ';
		}
		if (ans[25] >= i)
			cout << '*';
		else
			cout << ' ';
		cout << endl;
	}
	for (char c = 'A'; c < 'Z'; c++)
		cout << c << ' ';
	cout << 'Z';
	return 0;
}
