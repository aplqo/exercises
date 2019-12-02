#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=100000*2;

struct peo
{
    int n,w,c;
}p[maxn];

bool cmp(const peo &a,const peo &b)
{
    return a.c!=b.c? a.c>b.c : a.n<b.n;
}
int main()
{
    int n,r,q;
    cin>>n>>r>>q;
    n*=2;q--;
    for_each(p,p+n,[](peo &i)->void{cin>>i.c;});
    for_each(p,p+n,[](peo &i)->void{cin>>i.w;});
    {
        int i=0;
        for_each(p,p+n,[&i](peo &a){a.n=++i;});
    }
    for(int i=0;i<r;i++)
    {
        sort(p,p+n,cmp);
        for(peo *j=p,*k=p+1;k<p+n;k+=2,j+=2)
            if(j->w>k->w) j->c++;
            else k->c++;
    }
    sort(p,p+n,cmp);
    cout<<p[q].n;
    return 0;
}
