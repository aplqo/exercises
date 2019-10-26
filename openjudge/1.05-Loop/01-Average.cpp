#include<cstdio>
using std::scanf;
using std::printf;
int main()
{
	int a=0,n=0,t=0;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&t);
		a+=t;
	}
	float ans=(float)a/n;
	printf("%.2lf",ans);
	return 0;
}
