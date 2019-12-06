#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=30000;

int obj[maxn],ans;

int main()
{
    int n,w;
    cin>>w>>n;
    for(int *i=obj;i<obj+n;i++)
        cin>>*i;
    sort(obj,obj+n);
    int *l=obj,*r=obj+n-1;
    while (l<=r)
    {
        if(*l+*r<=w) l++,r--;
        else r--;
        ans++;
    }
    cout<<ans;
    return 0;
}