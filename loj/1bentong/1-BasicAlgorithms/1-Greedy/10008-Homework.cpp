#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
const int maxn=1000000,maxt=700000;

struct home
{
    int lim,mark;
}hw[maxn];
int ans;
int tim[maxt];

bool cmp(const home &a,const home &b)
{
    if(a.mark!=b.mark) return a.mark>b.mark;
    return a.lim>b.lim;
}
int getfree(int x)
{
    return tim[x]==x ? x: tim[x]=getfree(tim[x]);
}
int main()
{
    int n;
    cin>>n;
    for(home *i=hw;i<hw+n;i++)
        cin>>i->lim>>i->mark;
    sort(hw,hw+n,cmp);
    {
        home *i=max_element(hw,hw+n,[](home &a,home &b){return a.lim<b.lim;});
        iota(tim,tim+i->lim+1,0);
    }
    for(home *i=hw;i<hw+n;i++)
    {
        int t=getfree(i->lim);
        if(t==0) continue;
        tim[t]=getfree(t-1);
        ans+=i->mark;
    }
    cout<<ans;
    return 0;
}