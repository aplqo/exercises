#include <iostream>
#include <numeric>
#include <cctype>
#include <stack>
#include <algorithm>
using namespace std;

struct lop
{
	unsigned char v;
	int ex, thi;
};
inline int getC()
{
	char c;
	int x = 1;
	cin.ignore(3);
	cin >> c;
	if (c == '1') x = 0;
	if (cin.peek() == '^')
	{
		cin.ignore(1);
		cin >> x;
	}
	cin.ignore(1);
	return x;
}
inline void err(int x)
{
	for (int i = 0; i < x; i++)
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "ERR" << endl;
}
void Solve()
{
	bool var[27] = { false };
	int ans = 0, ex, thi = 0;
	int ln;
	stack<lop> s;
	cin >> ln;
	ex = getC();
	for (; ln; ln--)
	{
		char op;
		cin >> op;
		switch (op)
		{
		case 'F':
		{
			lop tmp = { 0,0,numeric_limits<int>::min() };
			char  beg[4], end[4];
			cin >> tmp.v >> beg >> end;
			tmp.v -= 'a';
			if (var[tmp.v])
			{
				err(ln);
				return;
			}
			var[tmp.v] = true;
			if (isdigit(beg[0]) && isdigit(end[0]) && atoi(beg) <= atoi(end))
				tmp.thi = 0;
			else if (isdigit(beg[0]) && end[0] == 'n')
				tmp.thi = 1;
			else if (beg[0] == 'n' && end[0] == 'n')
				tmp.thi = 0;
			tmp.ex = tmp.thi;
			s.push(tmp);
			break;
		}
		case 'E':
			if (s.empty())
			{
				err(ln);
				return;
			}
			var[s.top().v] = false;
			int t = s.top().ex;
			if (s.size() == 1)
				thi = max(thi, s.top().ex);
			s.pop();
			if (!s.empty())
				s.top().ex = max(s.top().ex, t + s.top().thi);
			break;
		}
		if (s.empty())
		{
			ans = max(ans, thi);
			thi = 0;
		}
	}
	if (!s.empty())
	{
		err(ln);
		return;
	}
	if (ex == ans)
		cout << "Yes" << endl;
	else
	{
		cout << "No";
#ifdef APDEBUG
		cerr << " " << ans;
#endif
		cout << endl;
	}
}
int main()
{
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
		Solve();
	return 0;
}
