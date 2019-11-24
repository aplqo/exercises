#include<iostream>
using namespace std;

int ans=1;
int main()
{
    int n,m,acc=0,t;
    cin>>n>>m;
    for(int i=0;i<n;i++)
    {
        cin>>t;
        if(acc+t>m)
        {
            ans++;
            acc=0;
        }
        acc+=t;
    }
    cout<<ans;
    return 0;
}