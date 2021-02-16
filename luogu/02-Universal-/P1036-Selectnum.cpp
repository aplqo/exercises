#include <cmath>
#include <iostream>
using namespace std;
int ans = 0;
int num[20], k, n;
int s;
bool isPrime(int n)
{
  for (int i = 2; i <= sqrt(n); i++)
    if (n % i == 0) return false;
  return true;
}
void fun(int i, int dep)
{
  if (dep >= k) {
    if (isPrime(s)) ans++;
    return;
  }
  for (; i < n; i++) {
    s += num[i];
    fun(i + 1, dep + 1);
    s -= num[i];
  }
}
int main()
{
  cin >> n >> k;
  for (int i = 0; i < n; i++)
    cin >> num[i];
  fun(0, 0);
  cout << ans;
  return 0;
}
