#include <iostream>
using namespace std;
const int m = 6 * 49 + 6 * 7 + 6;
#define rep for (int j = 0; j < 3; j++)
int main()
{
  int b7[3], b9[3];
  for (int i = 1; i <= m; i++) {
    int t7 = i, t9 = i;
    rep
    {
      b7[j] = t7 % 7;
      b9[j] = t9 / 81;
      if (b7[j] != b9[j]) goto f;
      t7 /= 7;
      t9 = (t9 * 9) % 729;
    }
    cout << i << endl;
    rep cout << b7[2 - j];
    cout << endl;
    rep cout << b9[j];
    return 0;
  f:;
  }
}
