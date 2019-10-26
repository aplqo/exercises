#include<iostream>
using namespace std;

struct dt
{
	int a,b;
	int x,y;
} c[10000];
int main()
{
	int n,ans=-1;
	cin>>n;
	for(dt* i=c;i<c+n;i++)
		cin>>i->a>>i->b>>i->x>>i->y;
	int x,y;
	cin>>x>>y;
	for(dt *i=c+n-1;i>=c;i--)
		if(x>=(i->a)&&x-(i->a)<=(i->x))
			if(y>=(i->b)&&y-(i->b)<=(i->y))
			{
				ans=i-c+1;
				break;
			}
	cout<<ans;
	return 0;
}
