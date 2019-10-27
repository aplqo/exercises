#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int n,k;
	cin>>n>>k;
	double rat=k/100.0+1;
	double g=0,price=200;
	for(int i=0;i<=20;i++)
	{
		g+=n;
		if(g>=price)
		{
			cout<<(i+1);
			return 0;
		}
		price*=rat;
	}
	cout<<"Impossible";
	return 0;
}
