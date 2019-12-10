#include<iostream>
#include<algorithm>
#include<numeric>
#include<cstdio>
#include<climits>
using namespace std;
const int maxn = 200000;

int fa[maxn], chl[maxn],dis[maxn];
int ans=INT_MAX;

int GetFather(int x)
{
	return fa[x]==x ? x:fa[x]=GetFather(fa[x]);
}
inline void merge(int x,int y)
{
	fa[GetFather(y)]=GetFather(x);
	chl[x]=y;
}
int main(int argc,char *argv[])
{
#ifdef APDEBUG
	freopen(argv[1],"w",stdout);
#endif
	int n, beg, t;
	cin >> n;
	iota(fa + 1, fa + n + 1, 1);
	for (int i = 1; i <= n; i++)
	{
		cin >> t;
		if (GetFather(t) == GetFather(i))
		{
			chl[i]=t;
			int td=1;
			for(int j=i;chl[j]!=i;j=chl[j],++td);
			if(td<ans)
				(ans=td),(beg=i);
		}
		merge(i,t);
	}
#ifdef APDEBUG
	t=beg;
	do
	{
		cout<<t<<' ';
		t=chl[t];
	} while (t!=beg);
#endif
#ifndef APDEBUG
	cout << ans;
#endif
	return 0;
}
