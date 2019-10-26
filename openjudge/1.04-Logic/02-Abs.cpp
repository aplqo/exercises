#include<cstdio>
using std::printf;
using std::scanf;
int main()
{
	double x;
	scanf("%lf", &x);
	if (x < 0)
		x = -x;
	printf("%.2lf", x);
	return 0;
}
