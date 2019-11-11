#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<chrono>
using std::cout;
using std::endl;
using std::chrono::steady_clock;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Too few arguments" << endl;
		return 0;
	}
	freopen(argv[2], "r",stdin);
	auto bef = steady_clock::now();
	system(argv[1]);
	auto aft = steady_clock::now();
	microseconds d = duration_cast<microseconds>(aft - bef);
	milliseconds d2=duration_cast<milliseconds>(aft-bef);
	cout << endl << "---------------" << endl;
	cout << "Program finished after " << d.count() << "us. ("<<d2.count()<<"ms)";
	if(d2.count()>=1000)
		cout<<endl<<"Warn: TLE may happen";
	return 0;
}
