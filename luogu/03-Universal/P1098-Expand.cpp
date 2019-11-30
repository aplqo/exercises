#include<iostream>
#include<cctype>
#include<string>
using namespace std;

int main()
{
	int p1,p2,p3;
	cin>>p1>>p2>>p3;
	string s;
	cin>>s;
	string::size_type pos=s.find('-');
	for(;pos!=string::npos;pos=s.find('-',pos))
	{
		char l=s[pos-1],r=s[pos+1];
		if(pos==0||r<=l||isalpha(l)^isalpha(r)||isdigit(l)^isdigit(r)) 
		{
			pos++;continue;
		}
		s.erase(pos,1);
		if(r-l==1) continue;
		switch(p1)
		{
			case 1:
				l=tolower(l);
				r=tolower(r);
				break;
			case 2:
				l=toupper(l);
				r=toupper(r);
				break;
			case 3:
				s.insert(pos,(r-l-1)*p2,'*');
				goto en;
		}
		if(p3==1)
		{
			for(char i=l+1;i<r;i++)
			{
				s.insert(pos,p2,i);
				pos+=p2;
			}
		}
		else
			for(char i=r-1;i>l;i--)
			{
				s.insert(pos,p2,i);
				pos+=p2;
			}
		en:;
	}
	cout<<s;
	return 0;
}
