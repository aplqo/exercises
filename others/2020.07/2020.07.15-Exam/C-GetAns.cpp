#include<fstream>
using namespace std;
const unsigned int maxn=2000;
const unsigned long long mod=1000000007,inv=500000004;

bool fail[maxn+1][maxn+1],vis[maxn+1][maxn+1];
unsigned int n,m;

struct visGuard
{
	visGuard(unsigned int r,unsigned int c):r(r),c(c)
	{
		vis[r][c]=true;
	}
	~visGuard()
	{
		vis[r][c]=false;
	}
	const unsigned int r,c;
};
unsigned long long dfs1(const unsigned int r,const unsigned int c,const bool typ)
{
	if(c>m||r>n||(vis[r][c]&&(r!=1||c!=1))||fail[r][c])
		return 0;
	if(c==m&&r==n)
		return typ? dfs1(1,1,false): 1;
	visGuard g(r,c);
	return (dfs1(r,c+1,typ)+dfs1(r+1,c,typ))%mod;
}
int main(int,char*argv[])
{
	ifstream in(argv[1]);
	ofstream ans(argv[2]);
	in>>n>>m;
	for(unsigned int i=1;i<=n;++i)
	{
		static char str[maxn+1];
		in>>str;
		for(unsigned int j=1;j<=m;++j)
			fail[i][j]=str[j-1]=='1';
	}
	if(fail[2][1]||fail[1][2]||fail[n-1][m]||fail[n][m-1]||fail[n][m]||fail[1][1])
	{
		ans<<0<<endl;
		return 0;
	}
	if((n<=2&&m==1)||(n==1&&m<=2))
	{
		ans<<1<<endl;
		return 0;
	}
	ans<<(dfs1(1,1,true)*inv)%mod<<endl;
	return 0;
}
