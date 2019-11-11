#include<iostream>
using namespace std;

long long ans[22][22];
int m,n,x,y;
inline void ex(int dx,int dy)
{
    auto t=[](int x1,int y1){if(x1>0&&x1<=m&&y1>0&&y1<=n) ans[x1][y1]=-1;};
    t(x-dx,y-dy);
    t(x-dx,y+dy);
    t(x+dx,y-dy);
    t(x+dx,y+dy);
}
int main()
{
    cin>>m>>n>>x>>y;
    ans[1][1]=1;
    m++;n++;x++;y++;
    ans[x][y]=-1;
    ex(2,1);ex(1,2);
    for(int i=1;i<=m;i++)
        for(int j=1;j<=n;j++)
        {
        	if(ans[i][j]==-1) continue;
            if(ans[i-1][j]!=-1) ans[i][j]+=ans[i-1][j];
            if(ans[i][j-1]!=-1) ans[i][j]+=ans[i][j-1];
        }
    cout<<ans[m][n];
    return 0;
}
