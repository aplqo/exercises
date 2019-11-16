#include<iostream>
#include<algorithm>
using namespace std;
const int N = 5005;

struct peo
{
	int num, score;
}p[N];
bool cmp(const peo& a, const peo& b)
{
	if (a.score != b.score) return a.score > b.score;
	return  a.num < b.num;
}
int main()
{
	int m, n;
	cin >> n >> m;
	for (peo* i = p; i < p + n; i++)
		cin >> i->num >> i->score;
	sort(p, p + n, cmp);
	int s = m * 1.5f;
	peo* l = find_if(p + s - 1, p + n, [&s](peo& a)->bool {return a.score != p[s - 1].score; });
	cout << p[s - 1].score << ' ' << l - p << endl;
	for_each(p, l, [](peo t)->void {cout << t.num << ' ' << t.score << endl; });
	return 0;
}