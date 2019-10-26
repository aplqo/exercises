#include<iostream>
using std::cin;
using std::cout;
int main()
{
	int x;
	cin>>x;
	if(x>0)
	{
		cout<<"positive";
	}
	else if(x<0)
	{
		cout<<"negative";
	}
	else
	{
		cout<<"zero";
	}
	return 0;
}
