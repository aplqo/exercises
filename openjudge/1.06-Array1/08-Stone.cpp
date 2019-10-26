#include<iostream>
using namespace std;
const int wins[5]={2,0,5,0,0};
int main()
{
	int n,a[100],b[100],na,nb;
	int ansa=0,ansb=0;
	cin>>n>>na>>nb;
	for(int i=0;i<na;i++) cin>>a[i];
	for(int i=0;i<nb;i++) cin>>b[i];
	for(int i=0;i<n;i++)
	{
		if(wins[a[i%na]]==b[i%nb])
			ansa++;
		if(wins[b[i%nb]]==a[i%na])
			ansb++;
	}
	if(ansa>ansb)
		cout<<"A";
	else if(ansb>ansa)
		cout<<"B";
	else
		cout<<"draw";
	return 0;
}
