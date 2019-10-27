#include<iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
	int n;
	cin>>n;
	double st;
	{
		int a,b;
		cin>>a>>b;
		st=(double)b/a;
	}
	for(int i=0;i<n-1;i++)
	{
		int a,b;
		double t,d;
		cin>>a>>b;
		t=(double)b/a;
		d=t-st;
		if(d>0.05)
		{
			cout<<"better"<<endl;
		}
		else if(d<-0.05)
		{
			cout<<"worse"<<endl;
		}
		else
		{
			cout<<"same"<<endl;
		}
	}
	return 0;
}
