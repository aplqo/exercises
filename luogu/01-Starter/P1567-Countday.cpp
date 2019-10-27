#include<iostream>
using namespace std;
int main()
{
	int n,t,beg=0,last=0,ans=0;
	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>t;
		if(t<last)
		{
			int l=i-beg;
			beg=i;
			if(l>ans) ans=l;
		}
		last=t;
	}
	cout<<ans;
	return 0;
}
