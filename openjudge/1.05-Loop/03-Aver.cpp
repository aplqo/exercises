#include<cstdio>
using std::scanf;
using std::printf;
int main()
{
	int n;
	float month=0,t;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		scanf("%f",&t);
		month+=t;
	}
	month/=n;
	printf("%.4f",month);
	return 0;
}
