#include<iostream>
#include<cmath>
using namespace std;

int main()
{
	int n,col,num;
	cin>>n;
	col=(-1+sqrt(1+8*n))/2;
	num=n-(col+1)*col/2;
	if(num) col++;
	if(col%2==0)
	{
		if(num==0) num=1;
		cout<<num<<'/'<<(col+1-num)<<endl;
	}
	else
	{
		if(num==0) num=col;
		cout<<(col+1-num)<<'/'<<num<<endl;
	}
	return 0;
}
