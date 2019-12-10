#include<iostream>
using namespace std;
typedef unsigned int num;
const int maxn=50005;

num sto[maxn],l;
unsigned int n,m;

bool test(num d)
{
    num lst=sto[0];
    unsigned int cur=0;
    for(unsigned int i=1;i<=n+1;i++)
    {
    	if(sto[i]-sto[lst]<d) cur++;
        else lst=i;
	}
    return cur<=m;
}
int main()
{
    cin>>l>>n>>m;
    sto[n+1]=l;
    for(num *i=sto+1;i<sto+n+1;i++)
        cin>>*i;
    num bl=0,br=l;
    while(bl<=br)
    {
        num mid=(bl+br)>>1;
        if(test(mid)) bl=mid+1;
        else br=mid-1;
    }
    cout<<br;
    return 0;
}
