#include<iostream>
using std::cin;
using std::cout;
int main()
{
	double h,ans=0;
	cin>>h;
	ans=h;
	for(int i=0;i<9;i++)
	{
		ans+=h;
		h/=2;
	}
	cout<<ans<<" "<<h/2;
	return 0;
}
