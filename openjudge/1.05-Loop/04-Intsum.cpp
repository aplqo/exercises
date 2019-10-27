#include<cstdio>
using std::scanf;
using std::printf;
int main()
{
	int n,t,sum=0;
	double ans;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&t);
		sum+=t;
	}
	ans=(double)sum/n;
	printf("%d %.5f",sum,ans);
	return 0;
}
