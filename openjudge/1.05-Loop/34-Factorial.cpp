#include<iostream>
using namespace std;
int main()
{
	int n,last=1,ans=0;
	cin>>n;
	for(int i=1;i<=n;i++)
	{
		last*=i;
		ans+=last;
	}
	cout<<ans;
	return 0;
}
