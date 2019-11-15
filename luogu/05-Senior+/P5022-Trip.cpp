#include<iostream>
#include<algorithm>
#include<climits>
#include<map>
using namespace std;
const int N = 5005;
typedef map<int, bool> graph;

graph gra[N];
bool visit[N];
int ans[N], * cur = ans, tans[N];

void dfs(int c)
{
	*cur = c;
	visit[c] = true;
	cur++;
	for (auto i : gra[c])
	{
		if (visit[i.first] || i.second) continue;
		dfs(i.first);
	}
}

int main()
{
	int n, m;
	cin >> n >> m;
	fill(tans, tans + n, INT_MAX);
	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;
		gra[a][b] = false;
		gra[b][a] = false;
	}
	if (m != n - 1)
		for (int i = 1; i <= n; i++)
			for (auto& j : gra[i])
			{
				if (j.first < i) continue;
				fill(visit, visit + n + 1, false);
				cur = ans;
				j.second = true;
				gra[j.first][i] = true;
				dfs(1);
				if (cur - ans == n)
					if (lexicographical_compare(ans, ans + n, tans, tans + n))
						copy(ans, ans + n, tans);
				j.second = false;
				gra[j.first][i] = false;
			}
	else
	{
		dfs(1);
		copy(ans, ans + n, tans);
	}
	for (int* i = tans; i < tans + n; i++)
		cout << *i << ' ';
	return 0;
}