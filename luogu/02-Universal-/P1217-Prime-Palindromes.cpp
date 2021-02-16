#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
long long primes[1000000], t, b, a;

bool isPrime(long long n)
{
  for (int i = 0; i < t && primes[i] <= sqrt(n); i++)
    if (n % primes[i] == 0) return false;
  return true;
}
void getPrimes()
{
  bool *f = new bool[100000];
  fill(f, f + 100000, false);
  for (int i = 2; i < sqrt(b); i++) {
    if (f[i]) continue;
    for (int j = 2; j <= sqrt(b) / i; j++)
      f[i * j] = true;
  }
  for (int i = 2; i < sqrt(b); i++)
    if (!f[i]) {
      primes[t] = i;
      t++;
    }
  delete f;
}
void test(int dep, int di, int num[8])
{
  long long d = 0;
  if (num[0] == 0) return;
  for (int i = 0; i < dep; i++)
    num[di - i - 1] = num[i];
  for (int j = 0; j < di; j++) {
    d *= 10;
    d += num[j];
  }
  if (d > b) exit(0);
  if (d < a) return;
  if (isPrime(d)) cout << d << endl;
}
void fun(int i, int dep = 0)
{
  static int num[8] = {0};
  if (dep == i % 2 + i / 2) {
    test(dep, i, num);
    return;
  }
  for (int j = 0; j < 10; j++) {
    num[dep] = j;
    fun(i, dep + 1);
  }
}
int main()
{
  cin >> a >> b;
  auto get = [](long long i) -> int {
    int j = 0;
    while (i) {
      j++;
      i /= 10;
    };
    return j;
  };
  getPrimes();
  int da = get(a), db = get(b);
  for (int i = da; i <= db; i++)
    fun(i);
  return 0;
}
