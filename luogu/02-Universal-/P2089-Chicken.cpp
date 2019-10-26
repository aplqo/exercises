#include<iostream>
#include<vector>
using namespace std;
struct a
{
	int i[10];
}ans;
vector<a> f;

template<int dep>
void func(int last)
{
	if(last<1) return;
	for(int i=1;i<4;i++)
	{
		ans.i[dep]=i;
		func<dep+1>(last-i);
	}
}
template<>
void func<10>(int last)
{
	if(last==0)
	{
		f.push_back(ans);
	}
}
int main()
{
	int n,num;
	cin>>n;
	func<0>(n);
	num=f.size();
	cout<<num<<endl;
	for(int i=0;i<num;i++)
	{
		for(auto j:f[i].i)
			cout<<j<<' ';
		cout<<endl;
	}
	return 0;
}
