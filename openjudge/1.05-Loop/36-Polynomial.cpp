#include<cstdio>
using namespace std;
int main()
{
	float x, ans = 1;
	float p = 1;
	int n;
	scanf("%f%d", &x, &n);
	for (int i = 1; i <= n; i++)
	{
		p *= x;
		ans += p;
	}
	printf("%.2f", ans);
	return 0;
}
