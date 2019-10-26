#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;

struct s{
	int a,b;
}ans;
char ch;
s get()
{
	char c;
	int t=0;
	bool n=false,v=false;
	s result={0,0};
	c=cin.get();
	n=c=='-';
	while(1)
	{
		if(isalpha(c))
		{
			v=true;
			ch=c;
		}
		else if(isdigit(c))
		{
			t*=10;
			t+=(unsigned char)c-(unsigned char)'0';
		}
		else
		{
			if(n) t=0-t;
			if(v) result.a+=t==0? 1:t;
			else result.b+=t;
			t=0;
			v=false;
			n=c=='-';
			if(c=='='||c=='\n') break;
		}
		c=cin.get();
	}
	return result;
}
int main()
{
	s a=get(),b=get();
	ans.a=a.a-b.a;
	ans.b=a.b-b.b;
	printf("%c=%.3lf",ch,(double)-ans.b/ans.a);
	return 0;
}
