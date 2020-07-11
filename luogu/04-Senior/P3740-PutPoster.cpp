#include<iostream>
#include<algorithm>
using namespace std;
const unsigned int maxn=1e7,maxm=1000;

class STree
{
public:
	void build(const unsigned int l,const unsigned int r,STree*(&cur))
	{
		lv=l;
		rv=r;
		mv=(l+r)>>1;
		val=0;
		if(r-l<=1)
			return;
		lc=++cur;
		lc->build(l,mv,cur);
		rc=++cur;
		rc->build(mv,r,cur);
	}
	void modify(const unsigned int l,const unsigned int r,const unsigned int val)
	{
		if(l==lv&&r==rv)
		{
			push(val);
			return;
		}
		pushdown();
		if(r<=mv)
			lc->modify(l,r,val);
		else if(l>=mv)
			rc->modify(l,r,val);
		else
		{
			lc->modify(l,mv,val);
			rc->modify(mv,r,val);
		}
	}
	unsigned int find(const unsigned int x)const
	{
		if(val||rv-lv==1)
			return val;
		return x<mv? lc->find(x) : rc->find(x);
	}
	
private:
	void push(const unsigned int laz)
	{
		this->val=laz;
	}
	void pushdown()
	{
		if(!val)
			return;
		lc->push(val);
		rc->push(val);
		val=0;
	}
		
	STree*lc,*rc;
	unsigned int lv,mv,rv;
	unsigned int val;
}SNodes[maxm*4+10],*SRoot=SNodes;
struct post
{
	unsigned int a,b;
}p[maxm+1];

static unsigned int discretize(const unsigned int m)
{
	static unsigned int dis[maxm*4+1];
	unsigned int* cur=dis;
	for(post*i=p+1;i<p+1+m;++i)
	{
		*(cur++)=i->a;
		*(cur++)=i->b;
		*(cur++)=i->a-1;
		*(cur++)=i->b+1;
	}
	sort(dis,cur);
	unsigned int *const en=unique(dis,cur);
	for(post*i=p+1;i<p+1+m;++i)
	{
		i->a=lower_bound(dis,en,i->a)-dis;
		i->b=lower_bound(dis,en,i->b)-dis;
	}
	return en-dis;
}
unsigned int solve(const unsigned int m)
{
	static unsigned int tmp[maxm*4+1];
	const unsigned int mv=discretize(m);
	{
		STree*c=SNodes;
		SRoot->build(0,mv,c);
	}
	for(post*i=p+1;i<p+1+m;++i)
		SRoot->modify(i->a,i->b+1,i-p);
	for(unsigned int i=0;i<mv;++i)
		tmp[i]=SRoot->find(i);
	sort(tmp,tmp+mv);
	return unique(tmp,tmp+mv)-1-tmp;
}

int main()
{
	unsigned int n,m;
	cin>>n>>m;
	for(post*i=p+1;i<p+1+m;++i)
		cin>>i->a>>i->b;
	cout<<solve(m)<<endl;
	return 0;
}

