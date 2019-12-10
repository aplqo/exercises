#include<iostream>
using namespace std;
const int maxn = 15;

int ans[maxn], n, ansn;
bool row[maxn], pos[maxn * 2], neg[maxn * 2];
bool* const mpos = pos + maxn, * const mneg = neg + maxn;

void dfs(int c)
{
	if (c > n)
	{
		if (ansn < 3)
		{
			for (int* i = ans + 1; i < ans + n + 1; i++)
				cout << *i << " ";
			cout << endl;
		}
		ansn++;
		return;
	}
	const static auto ass = [](int i, int cp, int cn, bool val)->void {
		row[i] = val; *(mpos + cp) = val; *(mneg + cn) = val;
	};
	for (int i = 1; i <= n; i++)
	{
		int cp = c - i, cn = c + i;
		if (row[i] || *(mpos + cp) || *(mneg + cn)) continue;
		ass(i, cp, cn, true);
		ans[c] = i;
		dfs(c + 1);
		ass(i, cp, cn, false);
	}
}
int main()
{
	cin >> n;
	dfs(1);
	cout << ansn;
	return 0;
}