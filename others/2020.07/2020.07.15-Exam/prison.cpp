#if !defined APTEST && !defined APDEBUG
#define Judge
#endif
#include<algorithm>
#include<cstdio>
#include<climits>
using namespace std;
const unsigned int maxn=16,maxs=1<<maxn;
#define lowbit(x) ((x)&-(x))

struct edge
{
	unsigned int to;
	edge*pre;
}ed[maxn*maxn+1];
edge*head[maxn+1];
bool vis[maxn+1];
unsigned int cnt[maxs+1],lg[maxs+1],f[maxs+1];

void addEdge(const unsigned int from,const unsigned int to)
{
	static edge*cur=ed;
	cur->to=to;
	cur->pre=head[from];
	head[from]=cur;
	++cur;
}
static void init(const unsigned int n)
{
	const unsigned int msk=1<<n;
	for(unsigned int i=0;i<=16;++i)
		lg[1<<i]=i;
	for(unsigned int i=1;i<msk;++i)
	{
		const unsigned int t=lowbit(i);
		cnt[i]=cnt[i^t];
		for(edge*j=head[lg[t]];j;j=j->pre)
			if((1<<(j->to))&i)
				++cnt[i];
	}
	fill(f+1,f+1+msk,UINT_MAX/2);
}
unsigned int dp(const unsigned int n,const unsigned int k)
{
	const unsigned int msk=(1<<n)-1;
	for(unsigned int i=0;i<=msk;++i)
	{
		for(unsigned int j=i;j;j=(j-1)&i)
			if(cnt[i^j]<=k)
				f[i]=min(f[i],f[j]+1);
		if(cnt[i]<=k)
			f[i]=min(f[i],1u);
	}
	return f[msk];
}

int main()
{
#ifdef Judge
	freopen("prison.in","r",stdin);
	freopen("prison.out","w",stdout);
#endif
	unsigned int n,m,k;
	scanf("%u%u%u",&n,&m,&k);
	for(unsigned int i=0;i<m;++i)
	{
		unsigned int x,y;
		scanf("%u%u",&x,&y);
		--x;
		--y;
		addEdge(x,y);
		addEdge(y,x);
	}
	init(n);
	printf("%u\n",dp(n,k));
	return 0;
}
