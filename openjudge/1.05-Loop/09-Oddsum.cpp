#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int m,n,ans=0;
	cin>>m>>n;
	for(int i=m;i<=n;i++)
	{
		if(i%2)
			ans+=i;
	}
	cout<<ans;
	return 0;
}
