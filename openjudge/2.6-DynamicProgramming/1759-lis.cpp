#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=1005;
typedef int numType;

numType a[maxn],f[maxn];
int main()
{
    int n;
    cin>>n;
    for(numType *i=a+1;i<a+n+1;++i)
        cin>>*i;
    fill(f+1,f+n+1,1);
    for(int i=1;i<=n;++i)
        for(int j=1;j<i;++j)
            if(a[i]>a[j])
                f[i]=max(f[j]+1,f[i]);
    cout<<*max_element(f+1,f+n+1);
    return 0;
}