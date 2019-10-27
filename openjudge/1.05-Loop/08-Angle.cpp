#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int n,ans,t;
	cin>>n;
	ans=(n-2)*180;
	for(int i=0;i<(n-1);i++)
	{
		cin>>t;
		ans-=t;
	}
	cout<<ans;
	return 0;
}
