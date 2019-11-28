#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=305;

struct stu
{
    int ch;
    int sum,n;
}st[maxn];

bool cmp(const stu &a,const stu &b)
{
    if(a.sum!=b.sum) return a.sum>b.sum;
    if(a.ch!=b.ch) return a.ch>b.ch;
    return a.n<b.n;
}
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<n+1;i++)
    {
        int t1,t2;
        cin>>st[i].ch>>t1>>t2;
        st[i].sum=st[i].ch+t1+t2;
        st[i].n=i;
    }
    sort(st+1,st+1+n,cmp);
    for_each(st+1,st+6,[](stu &a)->void{cout<<a.n<<' '<<a.sum<<endl;});
    return 0;
}