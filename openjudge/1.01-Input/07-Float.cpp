#include <cstdio>
#include <iostream>
using std::cin;
using std::printf;
int main()
{
	double d;
	cin >> d;
	printf("%f\n%.5f\n%e\n%g\n", d, d, d, d);
	return 0;
}

