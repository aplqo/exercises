#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
using namespace std;
//comment this if print to file
#define APDEBUG
const int maxn=25;

bool cmp(const string &a,const string &b)
{
    if(a.size()!=b.size()) return a.size()>b.size();
    for(unsigned int i=0;i<a.size();i++)
        if(a[i]!=b[i])
            return a[i]>b[i];
    return false;
}
int main()
{
    #ifndef APDEBUG
    freopen("president.in","r",stdin);
    freopen("president.out","w",stdout);
    #endif
    int n,ans;
    cin>>n;
    string ma;
    for(int i=1;i<n+1;i++)
    {
        string s;
        cin>>s;
        if(cmp(s,ma))
        {
            ma=s;
            ans=i;
        }
    }
    cout<<ans<<endl<<ma<<endl;
    return 0;
}