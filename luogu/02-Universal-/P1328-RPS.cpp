#include<iostream>
using namespace std;

enum
{
	W,L,P
};
const int wins[5][5]={
	{P,L,W,W,L},
	{W,P,L,W,L},
	{L,W,P,L,W},
	{L,L,W,P,W},
	{W,W,L,L,P}
};
int ra[200],rb[200],la,lb;
int ansa,ansb;

int main()
{
	int n;
	cin>>n>>la>>lb;
	for(int *i=ra;i<ra+la;i++)
		cin>>*i;
	for(int *i=rb;i<rb+lb;i++)
		cin>>*i;
	for(int i=0;i<n;i++)
	{
		switch(wins[ra[i%la]][rb[i%lb]])
		{
			case W:
				ansa++;
				break;
			case L:
				ansb++;
				break;
			default: break;
		}
	}
	cout<<ansa<<' '<<ansb;
	return 0;
}
