#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
const int maxn=100005;
typedef unsigned int num;

num a[maxn],pre[maxn];
unsigned int n,m;

bool test(num d)
{
    unsigned int lst=0,cur=1;
    for(unsigned int i=1;i<=n;i++)
        if(pre[i]-pre[lst]>d)
        {
            lst=i-1;
            cur++;
        }
    return cur<=m;
}
int main()
{
    cin>>n>>m;
    for(num *i=a+1;i<a+n+1;i++)
        cin>>*i;
    partial_sum(a+1,a+n+1,pre+1);
    num l=*max_element(a+1,a+n+1),r=pre[n];
    while(l<=r)
    {
        num mid=(l+r)>>1;
        if(test(mid)) r=mid-1;
        else l=mid+1;
    }
    cout<<l;
    return 0;
}
