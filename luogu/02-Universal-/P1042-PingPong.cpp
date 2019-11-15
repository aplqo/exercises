#include<iostream>
#include<cmath>
#include<string>
using namespace std;
const int N=6000;

struct rou
{
	int a,b;
}r1[N],r2[N];
rou *c1=r1,*c2=r2;

int fun(const string &s,int b)
{
	auto &tar=(b==21? c2 : c1);
	for(unsigned int i=0;i<s.size();i++)
	{
		switch(s[i])
		{
			case 'W':
				tar->a++;
				break;
			case 'L':
				tar->b++;
				break;
			case 'E':
				if (tar->a == 0 && tar->b == 0&&tar!=(b==21?c2:c1)) tar--;
				return 1;
		}
		if((tar->a>=b||tar->b>=b)&&abs(tar->a-tar->b)>=2)
			tar++;
	}
	return 0;
}
int main()
{
	while(true)
	{
		string s;
		getline(cin,s);
		fun(s,21);
		if(fun(s,11))
			break;
	}
	for(rou *i=r1;i<=c1;i++)
		cout<<i->a<<":"<<i->b<<endl;
	cout << endl;
	for(rou *i=r2;i<=c2;i++)
		cout<<i->a<<":"<<i->b<<endl;
	return 0;
}
