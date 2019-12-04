#include<iostream>
#include<algorithm>
#include<climits>
#include<cstdio>
using namespace std;

struct segment
{
    int a,b;
}s[1000000];
unsigned long ans;
bool cmp(const segment &a,const segment &b)
{
    if(a.b!=b.b) return a.b<b.b;
    return a.a>b.a;
}
int main()
{
    int n;
    cin>>n;
    for(segment *i=s;i<s+n;i++)
        cin>>i->a>>i->b;
    sort(s,s+n,cmp);
    int end=0;
    for(segment *i=s;i<s+n;i++)
        if(i->a>=end)
        {
            end=i->b;
            ans++;
        }
    cout<<ans;
    return 0;
}