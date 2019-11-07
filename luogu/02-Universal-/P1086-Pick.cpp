#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;

struct plants
{
	int x,y,nu;
} plan[401];
plants* cur=plan+1;
int ans;
int main()
{
    int m,n,time;
    cin>>m>>n>>time;
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
        {
            int t;
            cin>>t;
            if(t)
            {
                cur->x=i;
                cur->y=j;
                cur->nu=t;
                cur++;
            }
        }
    sort(plan+1,cur,[](const plants &a,const plants &b)->bool{return a.nu>b.nu;});

    plants *pos=plan+1,*lst=plan;
    plan[0].y=plan[1].y;
    plan[0].x=-1;
    while(pos<cur)
    {
        time-=abs(pos->x-lst->x)+abs(pos->y-lst->y)+1;
        if(time<pos->x+1) break;
        ans+=pos->nu;
        lst++;
        pos++;
    }
    cout<<ans;
    return 0;
}
