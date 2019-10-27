#include<iostream>
using namespace std;
int main()
{
	int a,b,ans=0,m=8;
	for(int i=0;i<7;i++)
	{
		cin>>a>>b;
		if(a+b>m)
		{
			ans=i+1;
			m=a+b;
		}
	}
	cout<<ans;
	return 0;
}
