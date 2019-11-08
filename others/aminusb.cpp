#include<cstdio>
#include<algorithm>
#include<cmath>
using namespace std;

int gcd(int a,int b)
{
    a=abs(a);b=abs(b);
    if(a<b) swap(a,b);
    int r=a%b;
    while(r)
    {
        a=b;
        b=r;
        r=a%b;
    }
    return b;
}
void solve()
{
    int a,b,c,d,ansx,ansy;
    scanf("%d/%d-%d/%d",&a,&b,&c,&d);
    ansy=b*d;
    ansx=a*d-b*c;
    int g=gcd(ansx,ansy);
    ansx/=g;ansy/=g;
    if(abs(ansy)==1)
        printf("%d\n",ansx/ansy);
    else
    {
        if(ansx*ansy<0) printf("%c",'-');
        printf("%d/%d\n",abs(ansx),abs(ansy));
    }
}
int main()
{
    int n;
    #ifndef DEBUG
    freopen("aminusb.in","r",stdin);
    freopen("aminusb.out","w",stdout);
    #endif
    scanf("%d",&n);
    for(int i=0;i<n;i++)
        solve();
    return 0;
}
