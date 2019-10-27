#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int a=1,b=1,c=a+b;
	int k;
	cin>>k;
	if(k<3)
	{
		cout<<1;
		return 0;
	}
	for(int i=3;i<k;i++)
	{
		int t=c;
		c=b+t;
		a=b;
		b=t;
	}
	cout<<c;
	return 0;
}

