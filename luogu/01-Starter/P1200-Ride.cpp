#include <iostream>
#include <string>
using namespace std;
int main()
{
	string group, star;
	int g = 1, s = 1;
	cin >> star >> group;
	for (auto i : group)
		g *= i - 'A' + 1;
	for (auto i : star)
		s *= i - 'A' + 1;
	if ((s % 47) == (g % 47))
		cout << "GO";
	else
		cout << "STAY";
	return 0;
}
