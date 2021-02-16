#include <iostream>
#include <string>
using namespace std;
const int maxn = 9;
typedef unsigned int numType;

numType seq[maxn] = {1}, n;

void fun(numType s, numType *cur)
{
  if (s == 0) {
    for (numType *i = seq + 1; i < cur - 1; i++)
      cout << *i << "+";
    cout << *(cur - 1) << endl;
    return;
  }
  for (numType i = *(cur - 1); i <= s && i < n; i++) {
    *cur = i;
    fun(s - i, cur + 1);
  }
}
int main()
{
  cin >> n;
  fun(n, seq + 1);
  return 0;
}
