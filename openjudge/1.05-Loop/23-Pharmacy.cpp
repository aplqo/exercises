#include<iostream>
using namespace std;
int main()
{
	int m,n,t,ans=0;
	cin>>m>>n;
	for(int i=0;i<n;i++)
	{
		cin>>t;
		if(m<t)
		{
			ans++;
			continue;
		}
		m-=t;
	}
	cout<<ans;
	return 0;
}
