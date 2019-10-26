#include <iostream>
using namespace std;
int ans=1;
int f[1001]={0,1};
int main()
{
	int n;
	cin>>n;
	for(int i=1;i<=n;i++)
	{
		f[i]=1;
		for(int j=1;j<=i/2;j++)
			f[i]+=f[j];
	}
	cout<<f[n];
	return 0;
}
