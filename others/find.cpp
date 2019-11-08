#include<iostream>
#include<numeric>
#include<cstdio>
using namespace std;

int grades[651],lo[651],num[651];
int main()
{
    int n,k;
    #ifndef DEBUG
    freopen("find.in","r",stdin);
    freopen("find.out","w",stdout);
    #endif
    cin>>n>>k;
    for(int i=0,l=1;i<n;i++)
    {
        int t;
        cin>>t;
        if(grades[t]==0)
        {
            num[t]=l;
            l++;
        }
        grades[t]++;
    }
    partial_sum(grades+1,grades+651,lo+1);
    for(int i=0;i<k;i++)
    {
        int t;
        cin>>t;
        if(grades[t]==0)
        {
            cout<<"Fail!"<<endl;
            continue;
        }
        cout<<num[t]<<" "<<grades[t]<<" "<<(n-lo[t])<<endl;
    }
    return 0;
}
