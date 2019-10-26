#include<cstdio>
using namespace std;
#define range(l,r,p) if(t>=l&&t<=r){ans[p]++;continue;}
int ans[4], n;
int main()
{
	scanf("%d", &n);
	int t;
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &t);
		range(0, 18, 0);
		range(19, 35, 1);
		range(36, 60, 2);
		ans[3]++;
	}
	for (int i = 0; i < 4; i++)
		printf("%.2f%%\n", 100.0f * ans[i] / n);
	return 0;
}
