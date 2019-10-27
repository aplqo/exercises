#include<iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
	int a,n,ans=1;
	cin>>a>>n;
	for(int i=0;i<n;i++)
		ans*=a;
	cout<<ans;
	return 0;
}
