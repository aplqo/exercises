#include<iostream>
using namespace std;
int main()
{
	int num[100],n=-1;
	do
	{
		n++;
		cin>>num[n];
	}while(num[n]);
	for(int j=n-1;j>=0;j--) cout<<num[j]<<" ";
	return 0;
}
