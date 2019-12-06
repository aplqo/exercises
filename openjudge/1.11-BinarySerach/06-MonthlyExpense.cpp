#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
typedef unsigned long long num;
const int maxn=100005;

unsigned int mon[maxn],n,m;
num sum[maxn];

bool test(num d)
{
    unsigned int lst=0,nu=1;
    for(unsigned int i=1;i<=n;i++)
        if(sum[i]-sum[lst]>d)
        {
            nu++;
            lst=i-1;
        }
    return nu<=m;
}
int main()
{
    cin>>n>>m;
    for(unsigned int *i=mon+1;i<mon+1+n;i++)
        cin>>*i;
    partial_sum(mon+1,mon+1+n,sum+1);
    num l=*max_element(mon+1,mon+1+n),r=sum[n];
    while(l<=r)
    {
        num mid=(l+r)>>1;
        if(test(mid)) r=mid-1;
        else l=mid+1;
    }
    cout<<l;
    return 0;
}
