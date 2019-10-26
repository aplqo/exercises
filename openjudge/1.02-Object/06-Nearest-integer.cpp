#include <iostream>
using std::cout;
using std::cin;

int main()
{
	float f;
	cin >> f;
	cout << static_cast<int>(f);
	cout.flush();
	return 0;
}
