#include<iostream>
#include<algorithm>
using namespace std;
#define dis(v,p,a) (v+(p-a))
typedef unsigned int num;
const int maxn = 5 * 1e5;

num a[maxn], c[maxn];
unsigned long long ans;
void merge(num* l, num* r)
{
	if (r <= l) return;
	num* mid = a + (((l - a) + (r - a)) / 2);
	merge(l, mid);
	merge(mid + 1, r);
	copy(l, r + 1, dis(c, l, a));
	num* i = dis(c, l, a), * j = dis(c, mid + 1, a);
	num* cmid = dis(c, mid, a), * cr = dis(c, r, a);
	for (num* k = l; k <= r; k++)
	{
		if (i > cmid) (*k = *j), (j++);
		else if (j > cr) (*k = *i), (i++);
		else if (*i > * j)
		{
			*k = *j; j++;
			ans += cmid - i + 1;
		}
		else (*k = *i), (i++);
	}
}
int main()
{
	int n;
	cin >> n;
	for_each(a, a + n, [](num& i)->void {cin >> i; });
	merge(a, a + n - 1);
	cout << ans;
	return 0;
}
