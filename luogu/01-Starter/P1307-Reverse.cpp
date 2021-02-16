#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int n, digit[10] = {0};
  int p = 0;
  cin >> n;
  if (n == 0) {
    cout << 0;
    return 0;
  }
  {
    int t = (n > 0) ? n : -n;
    while (t > 0) {
      digit[p] = t % 10;
      t /= 10;
      p++;
    }
  }
  int i;
  for (i = 0; i < p; i++) {
    if (digit[i] != 0) break;
  }
  if (n < 0) cout << "-";
  for (; i < p; i++) {
    cout << digit[i];
  }
  return 0;
}
