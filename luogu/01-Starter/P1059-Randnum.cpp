#include<iostream>
#include<algorithm>
using namespace std;
const int N = 105;

int a[N], ans[N],  * pans = ans;
int main()
{
	int n;
	cin >> n;
	for (int* i = a; i < a + n; i++)
		cin >> *i;
	sort(a, a + n);
	pans=unique_copy(a, a + n, ans);
	cout << (pans - ans) << endl;
	for_each(ans, pans, [](int t)->void {cout << t << ' '; });
	return 0;
}