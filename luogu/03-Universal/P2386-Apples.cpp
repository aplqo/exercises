#include<iostream>
using namespace std;

int fun(int m,int n)
{
    if(m<n) return fun(m,m);
    else if(m==0||n==1) return 1;
    else return fun(m-n,n)+fun(m,n-1);
}
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int a,b;
        cin>>a>>b;
        cout<<fun(a,b)<<endl;
    }
    return 0;
}
