#include<iostream>
using namespace std;
int main()
{
	int w, h;
	char c;
	bool s;
	auto p = [](char t, int w)->void {
		for (int i = 0; i < w; i++)
			cout << t;
	};
	cin >> h >> w >> c >> s;
	p(c, w);
	cout << endl;
	for (int i = 0; i < h - 2; i++)
	{
		cout << c;
		if (s)
			p(c, w - 2);
		else
			p(' ', w - 2);
		cout << c << endl;
	}
	p(c, w);
	return 0;
}
