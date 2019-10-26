#include<iostream>
using namespace std;
int s[9],ans;
bool has[10];
double a, b, c;
int toNum(int pos)
{
	return s[pos] * 100 + s[pos + 1] * 10 + s[pos + 2];
}
template<int dep>
inline void dfs()
{
	for (int i = 1; i < 10; i++)
	{
		if (has[i]) continue;
		has[i] = true;
		s[dep] = i;
		dfs<dep + 1>();
		has[i] = false;
	}
}
template<>
inline void dfs<6>()
{
	if ((double)toNum(3) / toNum(0) != b / a)
		return;
	for (int i = 1; i < 10; i++)
	{
		if (has[i]) continue;
		has[i] = true;
		s[6] = i;
		dfs<6 + 1>();
		has[i] = false;
	}
}
template<>
inline void dfs<9>()
{
	if ((double)toNum(6) / toNum(0) == c / a)
	{
		for (int i = 0; i < 9; i += 3)
			cout << s[i] << s[i + 1] << s[i + 2] << " ";
		cout << endl;
		ans++;
	}
}

int main()
{
	cin >> a >> b >> c;
	dfs<0>();
	if (ans == 0) cout << "No!!!";
	return 0;
}
