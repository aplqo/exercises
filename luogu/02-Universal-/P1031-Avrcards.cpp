#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
#define abs(x) (x>=0? (x):-(x))
const int maxn = 105;

int card[maxn];
int ans, avr;
int main()
{
	int n;
	cin >> n;
	for_each(card, card + n, [](int& i)->void {cin >> i; });
	avr = accumulate(card, card + n, 0);
	avr /= n;
	for_each(card, card + n, [](int& i) {i -= avr; });
	for (int* i = card, *j = card + 1; j < card + n; i++, j++)
	{
		if (abs(*i) > 0) ans++;
		*j += *i;
	}
	cout << ans;
	return 0;
}