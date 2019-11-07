#include<iostream>
#include<algorithm>
using namespace std;

int blocks[100000],ans;
int main()
{
    int n;
    cin>>n;
    for(int *i=blocks;i<blocks+n;i++) cin>>*i;
    int *beg=blocks;
    while(beg<blocks+n)
    {
        int *end=beg,m=0;
        end=find(beg,blocks+n,0);
        m=*min_element(beg,end);
        for_each(beg,end,[m](int &a){a-=m;});
        ans+=m;
        beg=find_if(beg,blocks+n,[](int a)->bool{return a!=0;});
    }
    cout<<ans;
    return 0;
}
