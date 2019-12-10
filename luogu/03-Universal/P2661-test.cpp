#include<iostream>
#include<fstream>
#include<algorithm>
#include<numeric>
using namespace std;
const int maxn = 200000;

unsigned int n, ans = 1;
unsigned int fa[maxn], child[maxn];
unsigned int pnum[maxn], pfst[maxn], pcur;

unsigned int GetFather(unsigned int x)
{
	return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline void merge(unsigned int x, unsigned int y)
{
	fa[GetFather(x)] = GetFather(y);
}
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Too few arguments";
		return 0;
	}
	ifstream dat(argv[1], ios::in);
	ifstream out;
	/*---analyze test data---*/
	dat >> n;
	iota(fa + 1, fa + 1 + n, 1);
	for (unsigned int i = 1; i <= n; i++)
	{
		unsigned int t;
		dat >> t;
		child[i] = t;
		merge(i, t);
	}
	for (unsigned int i = 1; i <= n; i++) GetFather(i);
	for (unsigned int i = 1; i <= n; i++)
	{
		if (find(pfst, pfst + pcur, GetFather(i)) != pfst + pcur) continue;
		pfst[pcur] = GetFather(i);
		pnum[pcur] = count(fa + 1, fa + 1 + n, GetFather(i));
		pcur++;
	}
	cout << "Node:" << n << endl;
	cout << "Find part:" << pcur << endl;
	for (unsigned int i = 0; i < pcur; i++)
	{
		cout << "Part #" << i + 1 << ": ";
		cout << "First Node:" << pfst[i];
		cout << " Node num:" << pnum[i];
		cout << endl;
	}
	cout << endl;
	dat.close();
	/*---Check debug output---*/
	out.open(argv[2], ios::in);
	unsigned int beg, cur, lst;
	out >> beg;
	cur = beg;
	while (!out.eof())
	{
		lst = cur;
		out >> cur;
		if(out.fail()) break;
		ans++;
		if (child[lst] != cur)
		{
			cout << "Fail: Child mismatch at " << cur;
			cout << " Last node: "<<lst<<" child[last]= " << child[lst] << endl;
			return 0;
		}
		if (cur == beg)
		{
			cout << "Fail: Begin node count twice!" << endl;
			return 0;
		}
	}
	cout << "Output in part #" << find(pfst, pfst + pcur, GetFather(beg)) - pfst + 1;
	cout << " First node:" << GetFather(beg) << endl;
	cout << "Pass: " << ans;
	return 0;
}