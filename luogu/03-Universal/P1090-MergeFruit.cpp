#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;

priority_queue<int,vector<int>,greater<int>> hea;
int ans;

int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int t;
        cin>>t;
        hea.push(t);
    }
    while(hea.size()>1)
    {
        int a,b;
        a=hea.top();hea.pop();
        b=hea.top();hea.pop();
        ans+=a+b;
        hea.push(a+b);
    }
    cout<<ans;
    return 0;
}
