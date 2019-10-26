#include<iostream>
#include<map>
using namespace std;
struct arg
{
		long long a,b,c;
};
bool operator<(arg a,arg b)
{
	if(a.a!=b.a) return a.a<b.a;
	if(a.b!=b.b) return a.b<b.b;
	return a.c<b.c;
}
map<arg,long long> f;

int fun(long long a,long long b,long long c)
{
	auto it=f.find({a,b,c});
	long long res=0;
	if(it!=f.end())
		return it->second;
	if(a<=0||b<=0||c<=0) return 1;
	if(a>20||b>20||c>20) return fun(20,20,20);
	if(a<b&&b<c)
	{
		res=fun(a,b,c-1)+fun(a,b-1,c-1)-fun(a,b-1,c);
	}
	else
		res=fun(a-1,b,c)+fun(a-1,b-1,c)+fun(a-1,b,c-1)-fun(a-1,b-1,c-1);
	f[{a,b,c}]=res;
	return res;
}
int main()
{
	long long a,b,c;
	cin>>a>>b>>c;
	while(a!=-1||b!=-1||c!=-1)
	{
		cout<<"w("<<a<<", "<<b<<", "<<c<<") = "<<fun(a,b,c)<<endl;
		cin>>a>>b>>c;
	}
	return 0;
}
